#include <stdlib.h>
#include <stdio.h>
 #include <string.h>
#include <xcb/xcb.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define GC_RED     0xFF0000
#define GC_GREEN   0x00FF00
#define GC_BLUE    0x0000FF
#define GC_BLACK   0x000000

/*
 *  This Demo Example is going to :
 *     1) Load JPEG File
 *     2) Display JPEG Image onto X display
 */

int main(int argc, char* argv[]) {

    if ( argc != 2 ) {
        printf("Usage : %s <JPEG File>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 1. Add Load JPEG File, fprce 4 components (RGBA) to make XCB alignment easier
    int width, height, channels;
    unsigned char *img_data = stbi_load(argv[1], &width, &height, &channels, 4);
    if ( !img_data ) {
        printf("ERROR: stbi_load() Could not load image\n");
        exit(EXIT_FAILURE);
    }


    // 2. Open the connection to the X server
    xcb_connection_t *connection = xcb_connect (NULL, NULL);
    if ( xcb_connection_has_error(connection)) {
        printf("ERROR: xcb_connect() Failed ...\n");
        exit(EXIT_FAILURE);
    }

    // 3. Get the first screen
    const xcb_setup_t      *setup  = xcb_get_setup (connection);
    xcb_screen_iterator_t   iter   = xcb_setup_roots_iterator (setup);
    xcb_screen_t           *screen = iter.data;
    if ( screen == NULL ) {
        printf("ERROR: Get First Screen Failed ...\n");
        exit(EXIT_FAILURE);
    }

    // 4. Add Reformat/Convert STB data (RGBA) to X11 format
    uint32_t pixmap_len = width * height * 4;
    uint32_t *xcb_pixmap = (uint32_t*)malloc(pixmap_len);
    for ( int i = 0; i < width * height; i++ ) {
        unsigned char r = img_data[i * 4];
        unsigned char g = img_data[i * 4 + 1];
        unsigned char b = img_data[i * 4 + 2];
        xcb_pixmap[i] = (r << 16) | (g << 8) | b;
    }


    // 5. Create the window
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
                       width, height,                 /* width, height       */
                       10,                            /* border_width        */
                       XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                       screen->root_visual,           /* visual              */
                       cw_mask, cw_values );          /* masks, not used yet */

    // 6. Create (allocate) Graphic Context (GC)
    xcb_gcontext_t gc_image = xcb_generate_id (connection);
    xcb_create_gc (connection, gc_image, window, 0, NULL);

    // 7. Map the window (make window visible)
    xcb_map_window (connection,  window );

    // 8. Flush the request to the server
    xcb_flush(connection);

    // 9. Write the Event loop
    xcb_generic_event_t *event;
    while ( (event = xcb_wait_for_event (connection)) ) {
        switch (event->response_type & ~0x80) {
        case XCB_EXPOSE: {
            xcb_expose_event_t *expose = (xcb_expose_event_t *)event;
            /* ...do stuff */
            if ( expose != NULL ) {
                // 1) Put Image to X display

                xcb_put_image (connection,
                               XCB_IMAGE_FORMAT_Z_PIXMAP,
                               window,
                               gc_image,
                               (uint16_t) width,
                               (uint16_t) height,
                               (int16_t) 0,
                               (int16_t) 0,
                               (uint8_t) 0,
                               screen->root_depth,
                               pixmap_len,
                               (const uint8_t *) xcb_pixmap);
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

    xcb_free_gc(connection, gc_image);
    // stbi_image_free(img_data);  <-- this function will cause memery crush if it has not been placed at right place
    free(xcb_pixmap);
    xcb_disconnect(connection);

    return 0;
}
