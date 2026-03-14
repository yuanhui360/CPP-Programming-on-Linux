#include <X11/Xlib.h>
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

    if ( argc != 1 ) {
        printf("Usage : %s \n", argv[0]);
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
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 100, 100, 400, 400,
                                        1, BlackPixel(display, screen), WhitePixel(display, screen));

    // 3. Load Fonts
    XFontStruct *font = XLoadQueryFont(display, "fixed");

    // 4. Select the type of inut events we want to handle
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // 5. Map (show) the window
    XMapWindow(display, window);

    // 6. Create ans Set Graphic Context
    GC gc_red = XCreateGC(display, window, 0, NULL);
    GC gc_blue = XCreateGC(display, window, 0, NULL);
    GC gc_green = XCreateGC(display, window, 0, NULL);
    GC gc_black = XCreateGC(display, window, 0, NULL);
    GC gc_text = XCreateGC(display, window, 0, NULL);

    // 7. Set Foreground Color using function XSetForeground()
    XSetForeground(display, gc_red, GC_RED);
    XSetForeground(display, gc_blue, GC_BLUE);
    XSetForeground(display, gc_green, GC_GREEN);
    XSetForeground(display, gc_black, GC_BLACK);

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

    XCloseDisplay(display);

    return 0;
}
