#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#define GC_RED     0xFF0000
#define GC_GREEN   0x00FF00
#define GC_BLUE    0x0000FF
#define GC_BLACK   0x000000

int main(int argc, char* argv[]) {

    XEvent event;
    (void) event;

    if ( argc != 2 ) {
        printf("Usage : %s <xbm file> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 1. Open connection to the X server
    Display *display = XOpenDisplay(NULL);
    if ( display == NULL ) {
        printf("ERROR: Cannot open display, Is X11 forwarding enable ?\n");
        exit(-1);
    }

    int screen = XDefaultScreen(display);

    // 2. Create a simple (unmapped) input/output window
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 100, 100, 400, 500,
                                        1, BlackPixel(display, screen), WhitePixel(display, screen));

    // 3. Load Fonts
    XFontStruct *font = XLoadQueryFont(display, "fixed");

    // 4. Select the type of inut events we want to handle
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // 5. Map (show) the window
    XMapWindow(display, window);

    // Add Load XBM data from file into a Pixmap

    Pixmap bitmap;
    unsigned int width = 0;
    unsigned int height = 0;

    // Hotspots (often unused for simple display)
    int x_hot = 0;
    int y_hot = 0;

    int rec = XReadBitmapFile(display, window, argv[1], &width, &height, &bitmap, &x_hot, &y_hot);
    if ( rec != BitmapSuccess ) {
        printf("ERROR: Unable to laod XBM Images from file\n");
        XCloseDisplay(display);
        return -1;
    }

    // 6. Create ans Set Graphic Context
    GC gc_red = XCreateGC(display, window, 0, NULL);
    GC gc_blue = XCreateGC(display, window, 0, NULL);
    GC gc_green = XCreateGC(display, window, 0, NULL);
    GC gc_black = XCreateGC(display, window, 0, NULL);
    GC gc_text = XCreateGC(display, window, 0, NULL);
    GC gc_xbm = XCreateGC(display, window, 0, NULL);

    // 7. Set Foreground Color using function XSetForeground()
    XSetForeground(display, gc_red, GC_RED);
    XSetForeground(display, gc_blue, GC_BLUE);
    XSetForeground(display, gc_green, GC_GREEN);
    XSetForeground(display, gc_black, GC_BLACK);

    XSetBackground(display, gc_xbm, WhitePixel(display, screen));

    // 8. Set Display Font
    XSetFont(display, gc_text, font->fid);

    // 9. Event Loop
    while (1) {
        XNextEvent(display, &event);
        if ( event.type == Expose) {
            // Draw 2 small rectangles
            XDrawRectangle(display, window, gc_red, 20, 20, 100, 100);
            XDrawRectangle(display, window, gc_green, 200, 20, 100, 100);

            // Fill 2 small rectangles
            XFillRectangle(display, window, gc_blue, 20, 280, 100, 100);
            XFillRectangle(display, window, gc_black, 200, 280, 100, 100);

            // Draw 3 different type lines
            XDrawLine(display, window, gc_blue, 20, 140, 350, 140);

            XSetLineAttributes(display, gc_red, 5, LineSolid, CapRound, JoinRound);
            XDrawLine(display, window, gc_red, 20, 180, 350, 180);

            XSetLineAttributes(display, gc_green, 5, LineOnOffDash, CapNotLast, JoinRound);
            XDrawLine(display, window, gc_green, 20, 220, 350, 220);

            // Draw a text Line
            XDrawString(display, window, gc_text, 160, 160, "Line Size 5", 12);

            // Copy the bitmap (1-bit) onto X window (N-bit)
            // XCopyPlan is used because bitmap are single-plane
            XCopyPlane(display, bitmap, window, gc_xbm, 0, 0, width, height, 20, 420, 1);
        }
        if ( event.type == KeyPress) {
                break;
        }
    }

    // 10. Clean Up and Close Connection
    XFreeGC(display, gc_red);
    XFreeGC(display, gc_blue);
    XFreeGC(display, gc_green);
    XFreeGC(display, gc_black);
    XFreeGC(display, gc_text);
    XFreeGC(display, gc_xbm);

    XFreePixmap(display, bitmap);

    XCloseDisplay(display);

    return 0;
}
