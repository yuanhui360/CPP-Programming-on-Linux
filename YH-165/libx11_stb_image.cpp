#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define GC_RED     0xFF0000
#define GC_GREEN   0x00FF00
#define GC_BLUE    0x0000FF
#define GC_BLACK   0x000000

/*
 *  This Example will do:
 *    1) Load jpg image file
 *    2) Display the image onto X window API
 */

int main(int argc, char* argv[]) {

    XEvent event;
    (void) event;

    if ( argc != 2 ) {
        printf("Usage : %s <xbm file> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 5. Add Load JPEG data from file (forcing 4 channels for RGBA)

    int width, height, channels;
    unsigned char *data = stbi_load(argv[1], &width, &height, &channels, 4);
    if ( !data ) {
        printf("ERROR: Unable to load JPEG Image from file\n");
        // XCloseDisplay(display);
        return -1;
    }

    // 1. Open connection to the X server
    Display *display = XOpenDisplay(NULL);
    if ( display == NULL ) {
        printf("ERROR: Cannot open display, Is X11 forwarding enable ?\n");
        exit(-1);
    }

    int screen = XDefaultScreen(display);

    // 2. Create a simple (unmapped) input/output window
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 100, 100, width, height,
                                        1, BlackPixel(display, screen), WhitePixel(display, screen));

    // 3. Select the type of inut events we want to handle
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // 4. Map (show) the window
    XMapWindow(display, window);

    // 6. Create ans Set Graphic Context
    GC gc_image = XCreateGC(display, window, 0, NULL);

    // 7. Add. Create XImage
    //    Note: use 32 bits per pixel as many X screen expected RGBA

    XImage *image = XCreateImage(display,
                                 DefaultVisual(display, screen),
                                 DefaultDepth(display, screen),
                                 ZPixmap,
                                 0,
                                 (char *)data,
                                 width,
                                 height,
                                 32,
                                 0);

    // 8. Event Loop
    while (1) {
        XNextEvent(display, &event);
        if ( event.type == Expose) {
            // 9. Put Image to X window
            XPutImage(display, window, gc_image, image, 0, 0, 0, 0, width, height);

        }
        if ( event.type == KeyPress) {
            break;
        }
    }

    // 10. Clean Up and Close Connection
    XFreeGC(display, gc_image);
    XDestroyImage(image);

    XCloseDisplay(display);

    return 0;
}
