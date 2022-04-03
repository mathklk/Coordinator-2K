#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QColorDialog>
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Coordinator-2K");
    _disp = new QLabel();
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(_disp);
    QPalette p;
    p.setColor(ui->labelColor->backgroundRole(), Qt::black);
    ui->labelColor->setAutoFillBackground(true);
    ui->labelColor->setPalette(p);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::_updateWorkMap Display the current _workMap according to current zoom factor in the gui
 */
void MainWindow::_displayWorkMap(void)
{
    if (nullptr == _workMap) {
        return;
    }
    if (!_livePreview) {
        qDebug() << "_displayWorkMap called although preview is off!";
        return;
    }

    QPixmap const dispMap = _workMap->scaled(
        ui->scrollArea->size() * _dispZoomFactor,
        Qt::AspectRatioMode::KeepAspectRatioByExpanding,
        Qt::TransformationMode::SmoothTransformation
    );
    _disp->setPixmap(dispMap);
}

/**
 * @brief MainWindow::on_pushButtonSelectImage_clicked Load image
 */
void MainWindow::on_pushButtonSelectImage_clicked()
{
    _sourceImageFileName = QFileDialog::getOpenFileName(
        this,
        "Select Source Image",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp)"
    );
    if (_sourceImageFileName.isNull()) {
        return;
    }

    _sourceImagePixmap = QPixmap(_sourceImageFileName);
    if (_livePreview) {
        _loadCleanScaled();
        _drawGrid();
        _displayWorkMap();
    }
    ui->currentSourceFileLabel->setText("Current Source File: " + _sourceImageFileName);
    ui->pushButtonExport->setEnabled(true);

}

void MainWindow::_loadCleanScaled(void)
{
    QSize const sourceSize = _sourceImagePixmap.size();
    QSize const scaledSize = sourceSize * _scaleFactor;
    if (nullptr != _workPainter) { // it is important to first delete the QPainter, then the QPixmap it paints on
        _workPainter->end();
        delete _workPainter;
    }
    if (nullptr != _workMap) {
        delete _workMap;
    }
    _workMap = new QPixmap(_sourceImagePixmap.scaled( // This operation takes some time
        scaledSize,
        Qt::AspectRatioMode::KeepAspectRatioByExpanding,
        Qt::TransformationMode::FastTransformation
    ));
    _workPainter = new QPainter(_workMap);
}

void MainWindow::_drawGrid(void) {
    // number of virtual pixels x and y
    int const tx = _sourceImagePixmap.size().width();
    int const ty = _sourceImagePixmap.size().height();

    int const actualHeight = _workMap->size().height();
    int const actualWidth = _workMap->size().width();

    QPen pen;
    pen.setColor(_color);
    pen.setWidth(ui->spinBoxLineWidth->value());
    _workPainter->setPen(pen);

    // Drawing the vertical lines by going through x
    for (int x = 0; x < tx; ++x) {
        int const actualX = x * _scaleFactor;
        _workPainter->drawLine(actualX, 0, actualX, actualHeight);
    }
    for (int y = 0; y < ty; ++y) {
        int const actualY = y * _scaleFactor;
        _workPainter->drawLine(0, actualY, actualWidth, actualY);
    }

    QFont font = _workPainter->font();
    font.setPixelSize(_fontSize);
    _workPainter->setFont(font);

    int nPix = 0;
    for (int x = 0; x < tx; ++x) {
        for (int y = 0; y < tx; ++y) {
            int const actualX = x * _scaleFactor;
            int const actualY = y * _scaleFactor;
            QRect const rect(actualX + 5, actualY + 2, _scaleFactor, _scaleFactor);
            _workPainter->drawText(
                rect,
                0,
                "Nr." + QString::number(nPix) + "\n"
                "X=" + QString::number(_offsetTopLeftX + x) + "\n"
                "Y=" + QString::number(_offsetTopLeftY + y)
            );

            nPix++;
        }
    }
}

void MainWindow::on_pushButtonExport_clicked()
{
    ui->labelDone->setText("Exporting...");
    QString const saveFileName = QFileDialog::getSaveFileName(
        this,
        "Select Export Location",
        "",
        "PNG Image (*.png)"
    );

    if (!_livePreview) { // i.e. theres no up-to-date preview already drawn
        _loadCleanScaled();
        _drawGrid();
    }

    QFile saveFile(saveFileName);
    saveFile.open(QIODevice::WriteOnly);
    _workMap->save(&saveFile, "PNG");
    ui->labelDone->setText("Exported to " + saveFileName);
}

/**
 * @brief Zooming in and out
 */
void MainWindow::on_pushButtonZoomIn_clicked()
{
    _dispZoomFactor *= _dispZoomAdvanceFactor;
    if (_livePreview) {
        _displayWorkMap();
    }
}

void MainWindow::on_pushButtonZoomOut_clicked()
{
    _dispZoomFactor /= _dispZoomAdvanceFactor;
    if (_livePreview) {
        _displayWorkMap();
    }
}

void MainWindow::on_zoomAdvanceSpinbox_valueChanged(double arg1)
{
    _dispZoomAdvanceFactor = arg1;
}

void MainWindow::on_spinBoxOffsetX_valueChanged(int arg1)
{
    _offsetTopLeftX = arg1;
    if (_livePreview) {
        _loadCleanScaled();
        _drawGrid();
        _displayWorkMap();
    }
}

void MainWindow::on_spinBoxOffsetY_valueChanged(int arg1)
{
    _offsetTopLeftY = arg1;
    if (_livePreview) {
        _loadCleanScaled();
        _drawGrid();
        _displayWorkMap();
    }
}

void MainWindow::on_spinBoxFontSize_valueChanged(int arg1)
{
    _fontSize = arg1;
    if (_livePreview) {
        _loadCleanScaled();
        _drawGrid();
        _displayWorkMap();
    }
}

void MainWindow::on_checkBoxLivePreview_stateChanged(int arg1)
{
    _livePreview = arg1;
    if (_livePreview && (nullptr != _workMap)) {
        _loadCleanScaled();
        _drawGrid();
        _displayWorkMap();
    }
}

void MainWindow::on_pushButtonColor_clicked()
{
    QColor const chosenColor = QColorDialog::getColor(Qt::black, this, "Chose color");
    if (chosenColor.isValid()) {
        _color = chosenColor;
        QPalette p;
        p.setColor(ui->labelColor->backgroundRole(), _color);
        ui->labelColor->setAutoFillBackground(true);
        ui->labelColor->setPalette(p);

        if (_livePreview) {
            _loadCleanScaled();
            _drawGrid();
            _displayWorkMap();
        }
    }
}

void MainWindow::on_spinBoxLineWidth_valueChanged(int)
{
    if (_livePreview) {
        _loadCleanScaled();
        _drawGrid();
        _displayWorkMap();
    }
}
