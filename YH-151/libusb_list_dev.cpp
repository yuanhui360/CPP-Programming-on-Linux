#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include "libusb_desc.h"

/*
 * Find and List all USB devices Attached to system
 *
 *  ---- SEQ ------ VID:PID ------ - TYPE - ------- Class/Sub Class ------- ----- Speed -----
 *  Device 1 -- VID:PID(8087:0024) USB 2.0. <Hub Device/Independent Device> speed HIGH (480M)
 */

int main(int argc, char* argv[])
{
    int rc;
    if (argc != 0 || argv != nullptr ) {
    }

    rc = libusb_init_context(NULL, NULL, 0);   // Initialize libusb
    if (rc < 0)
        return rc;

    // 1)  get library version and print
    const struct libusb_version* version;
    version = libusb_get_version();
    printf("--> %s v%d.%d.%d.%d\n", version->describe, version->major,
            version->minor, version->micro, version->nano);

    libusb_device **devs;                              // <-- list of USB devices
    ssize_t cnt = libusb_get_device_list(NULL, &devs); // Get list of connected USB devices
    if (cnt < 0){
        libusb_exit(NULL);
        return (int) cnt;
    }
    printf("Number of USB devices found : %d \n", (int)cnt);


    // 3) Loop print USB device info
    Libusb_Desc    desc_obj;
    for ( int i =0; devs[i] != NULL; i++ ) {
        libusb_device *dev = devs[i];
        struct libusb_device_descriptor desc;
        int rc = libusb_get_device_descriptor(dev, &desc);
        if ( rc != LIBUSB_SUCCESS ) {
            printf("Device %d - failed to get device descriptor", i);
            continue;
        }

        int speed_code = libusb_get_device_speed(dev);
        printf("Device %d -- VID:PID(%04x:%04x) %s <%s/%s> %s\n",
                i, desc.idVendor, desc.idProduct,
                desc_obj.bcdUSB_desc(desc.bcdUSB),
                desc_obj.class_desc(desc.bDeviceClass),
                desc_obj.class_desc(desc.bDeviceSubClass),
                desc_obj.speed_desc(speed_code));
    }


    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);                        // Deinitialize libusb
    return 0;
}

    // 2) Get total USB devices and list array
