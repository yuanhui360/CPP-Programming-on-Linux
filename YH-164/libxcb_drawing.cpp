#include <stdlib.h>
#include <stdio.h>
 #include <string.h>
#include <xcb/xcb.h>

#define GC_RED     0xFF0000
#define GC_GREEN   0x00FF00
#define GC_BLUE    0x0000FF
#define GC_BLACK   0x000000

/*
 *  This Demo Example is going to :
 *     1) Drawing 2 rectangles (red and green)
 *     2) Drawing 2 lines (red and Green)
 *     4) Drawing 1 filled rectangle (blue)
 *     4) Drawing 1 Text Message get from command line argument
 */

int main(int argc, char* argv[]) {

    if ( argc != 2 ) {
        printf("Usage : %s <Drwaing Message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    size_t msg_len = strlen(argv[1]);
    printf("INFO: %s (%d)\n", argv[1], (int)msg_len);

    // 1. Open the connection to the X server
    xcb_connection_t *connection = xcb_connect (NULL, NULL);
    if ( xcb_connection_has_error(connection)) {
        printf("ERROR: xcb_connect() Failed ...\n");
        exit(EXIT_FAILURE);
    }

    // 2. Get the first screen
    const xcb_setup_t      *setup  = xcb_get_setup (connection);
    xcb_screen_iterator_t   iter   = xcb_setup_roots_iterator (setup);
    xcb_screen_t           *screen = iter.data;
    if ( screen == NULL ) {
        printf("ERROR: Get First Screen Failed ...\n");
        exit(EXIT_FAILURE);
    }

    // 3. Create the window
    xcb_window_t window = xcb_generate_id (connection);
    uint32_t  cw_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t  cw_values[2] = {
                screen->white_pixel,
                XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};

    xcb_create_window (connection,                    /* Connection          */
                       XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                       window,                        /* window Id           */
                       screen->root,                  /* parent window       */
                       0, 0,                          /* x, y                */
                       400, 500,                      /* width, height       */
                       10,                            /* border_width        */
                       XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                       screen->root_visual,           /* visual              */
                       cw_mask, cw_values );          /* masks, not used yet */

    // 4. Create (allocate) Graphic Context (GC)
    xcb_gcontext_t gc_green = xcb_generate_id (connection);
    xcb_gcontext_t gc_red   = xcb_generate_id (connection);
    xcb_gcontext_t gc_blue  = xcb_generate_id (connection);

    uint32_t gc_mask = XCB_GC_FOREGROUND | XCB_GC_LINE_WIDTH | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t gc_green_values[3] = {GC_GREEN, 5, 0};
    uint32_t gc_red_values[3]   = {GC_RED, 1, 0};
    uint32_t gc_blue_values[3]  = {GC_BLUE, 1, 0};

    xcb_create_gc (connection, gc_green, window, gc_mask, gc_green_values);
    xcb_create_gc (connection, gc_red, window, gc_mask, gc_red_values);
    xcb_create_gc (connection, gc_blue, window, gc_mask, gc_blue_values);

    // 5. Open and Load font (using standard fixed font)
    xcb_font_t font = xcb_generate_id (connection);
    const char *font_name = "fixed";
    xcb_open_font(connection, font, strlen(font_name), font_name);

    xcb_gcontext_t gc_text  = xcb_generate_id (connection);
    uint32_t gc_text_mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
    uint32_t gc_text_values[3] = {
        screen->black_pixel,   // Foreground color
        screen->white_pixel,   // Background color
        font};                 // font
    xcb_create_gc (connection, gc_text, window, gc_text_mask, gc_text_values);

    // 6. Map the window (make window visible)
    xcb_map_window (connection,  window );

    // 7. Flush the request to the server
    xcb_flush(connection);

    // 8. Define Graphic Objects
    xcb_rectangle_t rect_red = {20, 20, 100, 100};
    xcb_rectangle_t rect_green = {200, 20, 100, 100};
    xcb_rectangle_t rect_blue  = {20, 200, 200, 100};
    xcb_point_t line_red[2] = {{20,140}, {350, 140}};
    xcb_point_t line_green[2] = {{20, 160}, {350, 160}};

    // 9. Write the Event loop
    xcb_generic_event_t *event;
    while ( (event = xcb_wait_for_event (connection)) ) {
        switch (event->response_type & ~0x80) {
        case XCB_EXPOSE: {
            xcb_expose_event_t *expose = (xcb_expose_event_t *)event;
            /* ...do stuff */
            if ( expose != NULL ) {
                // 1) Drawing 2 rectangles (red and green)
                xcb_poly_rectangle(connection, window, gc_red, 1, &rect_red);
                xcb_poly_rectangle(connection, window, gc_green, 1, &rect_green);

                // 2) Drawing 2 lines (red and green)
                xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN, window, gc_red, 2, line_red);
                xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN, window, gc_green, 2, line_green);

                // 3) Drawing 1 fille rectangle (blue)
                xcb_poly_fill_rectangle(connection, window, gc_blue, 1, &rect_blue);

                // 4) Drawing text message get from command line argument
                xcb_image_text_8(connection, strlen(argv[1]), window, gc_text, 20, 320, argv[1]);

                xcb_flush(connection);
            }
            break;
        }
        case XCB_KEY_PRESS: {
            xcb_key_press_event_t *press = (xcb_key_press_event_t *)event;
            /* ...do stuff */
            if ( press != NULL ) {
                // Exit Loop
                goto end_loop;
            }
            break;
        }
        default:
            /* Unknown event type, ignore it */
            break;
        }

        free (event);
    }
  
end_loop:
    // 10. Clean Up and Disconnect

    xcb_free_gc(connection, gc_red);
    xcb_free_gc(connection, gc_green);
    xcb_free_gc(connection, gc_blue);
    xcb_free_gc(connection, gc_text);

    xcb_close_font(connection, font);
    xcb_disconnect(connection);

    return 0;
}
