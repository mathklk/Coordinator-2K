#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>
#include <QPainter>
#include <QPixmap>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSelectImage_clicked();

    void on_pushButtonZoomIn_clicked();

    void on_pushButtonZoomOut_clicked();

    void on_zoomAdvanceSpinbox_valueChanged(double arg1);

    void on_spinBoxOffsetX_valueChanged(int arg1);

    void on_spinBoxOffsetY_valueChanged(int arg1);

    void on_spinBoxFontSize_valueChanged(int arg1);

    void on_checkBoxLivePreview_stateChanged(int arg1);

    void on_pushButtonExport_clicked();

    void on_pushButtonColor_clicked();

    void on_spinBoxLineWidth_valueChanged(int arg1);

private:
    void _loadCleanScaled(void);
    void _drawGrid(void);
    void _displayWorkMap(void);

private:
    Ui::MainWindow *ui;
    QLabel* _disp = nullptr;
    double _dispZoomAdvanceFactor = 1.2f;
    double _dispZoomFactor = 1;
    bool _livePreview = true;

    int _scaleFactor = 100;
    int _offsetTopLeftX = 0;
    int _offsetTopLeftY = 0;
    int _fontSize = 24;
    QColor _color = Qt::black;

    QString _sourceImageFileName = "";
    QPixmap _sourceImagePixmap;
    QPixmap* _workMap = nullptr;
    QPainter* _workPainter = nullptr;
};
#endif // MAINWINDOW_H
