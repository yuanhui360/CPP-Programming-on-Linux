#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include "libusb_desc.h"

static void print_devices_info(libusb_device **devs)
{
    libusb_device *dev;
    Libusb_Desc    desc_obj;

    /*
     *  ---- SEQ ------ VID:PID ------ - TYPE - ------- Class/Sub Class ------- ----- Speed -----
     *  Device 1 -- VID:PID(8087:0024) USB 2.0. <Hub Device/Independent Device> speed HIGH (480M)
     */

    int i = 0;
    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int rc = libusb_get_device_descriptor(dev, &desc);
        if ( rc != LIBUSB_SUCCESS ) {
            printf("Device %d - failed to get device descriptor", i);
            return;
        }

        int speed_code = libusb_get_device_speed(dev);
        printf("Device %d -- VID:PID(%04x:%04x) %s <%s/%s> %s\n",
                i, desc.idVendor, desc.idProduct,
                desc_obj.bcdUSB_desc(desc.bcdUSB),
                desc_obj.class_desc(desc.bDeviceClass),
                desc_obj.class_desc(desc.bDeviceSubClass),
                desc_obj.speed_desc(speed_code));
    }
}

/*
 * List all USB devices Attached to system
 */

int main(int argc, char* argv[])
{
    int rc;
    if (argc != 0 || argv != nullptr ) {
    }

    rc = libusb_init(NULL);   // parameter libusb_context pointer
    if (rc < 0)
        return rc;

    const struct libusb_version* version;
    version = libusb_get_version();
    printf("Using libusb v%d.%d.%d.%d\n", version->major,
            version->minor, version->micro, version->nano);

    libusb_device **devs;    // <-- Structure representing a USB device detected on the system.
    ssize_t cnt = libusb_get_device_list(NULL, &devs); // Get list of connected USB devices
    if (cnt < 0){
        libusb_exit(NULL);
        return (int) cnt;
    }
    printf("Number of USB devices found : %d \n", (int)cnt);

    print_devices_info(devs);
    libusb_free_device_list(devs, 1);

    libusb_exit(NULL);
    return 0;
}
