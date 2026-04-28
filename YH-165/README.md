Since X11's native functions only understand the XBM (1-bit) and XPM (multi-color) formats, it cannot read a JPG directly using standard libX11 or libxcb. 
This short video introduced a library, libstb that enable X API to load, decoding different image files for X window application using libx11/libxcb.

Libstb Image processing allows your program to decoding image format like PG, PNG, TGA, BMP, PSD, GIF, HDR, PIC etc. 

The example, it's going to write a C/C++ example to processing one of most common used image format file using libstb for libx11/libxcb API. The pixmap 
format in our example, we chose XPIXMAP.

In xcb_put_image(), XCB_IMAGE_FORMAT_Z_PIXMAP is the standard way to represent  image data where all the bits for a single pixel are stored contiguously 
in memory.

In Z_PIXMAP, the image is a simple linear array of pixels. If you have a 32-bit depth:
         Pixel 1: [Blue][Green][Red][Alpha] (4 bytes)
         Pixel 2: [Blue][Green][Red][Alpha] (4 bytes)
The "Z" in Z_PIXMAP stands for the "Z-axis" (depth).
         For a 24-bit or 32-bit visual, Z_PIXMAP expects each pixel to occupy a full word
         (usually 32 bits) to maintain memory alignment.
         Even if your image is only 24-bit RGB, X11 often expects a 32-bit Z_PIXMAP
         where the 4th byte is simply padding (ignored).

 The Alternative: XY_PIXMAP is a Planar format.     It sends the image as a series of bitmaps (planes).

 The video link of this example : https://youtu.be/lYo-OOGWaWo
