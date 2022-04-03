# Coordinator-2K

A gui tool that writes each pixels coordinates into the pixel.

Very useful for planning pixelart on r/place.

Application screenshot:

<img title="" src="doc/application_screenshot.png" alt="Screenshot of the application" width="594">

# Features

✔️Intuitive GUI Application

✔️Zoomable & Scrollable Preview Window

✔️Adjustable x-y-offset

✔️Adjustable font size

✔️Adjustable color for grid and text

✔️Adjustable line width

✔️Live Preview

✔️Can work with PNG, JPEG and BMP source files

✔️Written in C++/Qt, cross-platform, better performance than interpreted alternatives (e.g. Python) 

# Limitations

- Requires *lots* of memory when working with large source images

# Usage

A Pre-compiled version can be downloaded from the releases page.

Run the executable, the application should look similar to the screenshot above.

Click the button "Select Source Image". Chose the image you would like to coordinate.

Note: If you're working with a large image, you may want to disable live preview first.

Note: The source image **must not** be prescaled. Coordinator-2K scales the image x100 so that the text is readable. The source image must have one pixel per pixel.

Now you can adjust the settings on the left side. If live preview is enabled, the preview should update instantly.

Click export and chose where to save the output file.

Note: If live preview was disabled, then the output image will only now be generated. This may take a while depending on how much memory you have and how large the source image is. **The app may appear unresponsive, but it doesn't mean it crashed**. The app is single-threaded because multithreading is awful and this project is just for fun.


