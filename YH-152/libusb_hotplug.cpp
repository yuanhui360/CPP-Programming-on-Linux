#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libusb-1.0/libusb.h>
#include "libusb_desc.h"

#define INQUIRY_LENGTH                0x24

int event_count = 0;
libusb_device_handle *handle = NULL;

// Check if kernal drive is active on a interface libusb_kernel_driver_active()

void LIBUSB_CALL print_usb_info(libusb_device *dev)
{
    (void) dev;

    // Get Device descriptor libusb_get_device_descriptor()
    Libusb_Desc        desc_obj;
    struct libusb_device_descriptor     desc;
    int rc = libusb_get_device_descriptor(dev, &desc);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Device Attached\n");
        printf("Error getting device descriptor : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }

    // Display hotplug SUB device descriptor level info
    printf("Device attached: VID:PID [%04x:%04x] -- %s --> Number Configuration : %u\n",
            desc.idVendor, desc.idProduct, desc_obj.descriptor_type_desc(desc.bDescriptorType),
            desc.bNumConfigurations);
    printf("  <%s/%s> %s %s ",
            desc_obj.class_desc(desc.bDeviceClass),
            desc_obj.class_desc(desc.bDeviceSubClass),
            desc_obj.bcdUSB_desc(desc.bcdUSB),
            desc_obj.speed_desc(libusb_get_device_speed(dev)));

    // Check Open USB device accessibility libusb_open() libusb_close()
    if ( handle ) {
        libusb_close(handle);
        handle = NULL;
    }
    rc = libusb_open(dev, &handle);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("--> No access to device: %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }
    printf("--> libusb_open() Success \n");

    // Get Device (current) Configuration level parameters
    struct libusb_config_descriptor     *conf_desc;
    rc = libusb_get_active_config_descriptor(dev, &conf_desc);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("  Failed to get current configuration descriptor\n");
        return;
    }
    printf("  --> %s --> Number Interfaces: %u\n", desc_obj.descriptor_type_desc(conf_desc->bDescriptorType),
            conf_desc->bNumInterfaces);

    // Loop Get Configuration Interface level parameters
    for ( int i = 0; i < conf_desc->bNumInterfaces; i++ ) {
        struct libusb_interface       interface = conf_desc->interface[i];
        printf("  --> Interface[%d] --> Number Altsetting (Interface Descriptors): %u\n", i, interface.num_altsetting);
    // --> Loop Get Device Configuration->Interface->altsetting level parameters
        for ( int j = 0; j < interface.num_altsetting; j++ ) {
            struct libusb_interface_descriptor altSet = interface.altsetting[j];
            printf("      --> Altsetting[%d] --> %s <%s/%s> --> Number endpointd: %u\n", j,
                    desc_obj.descriptor_type_desc(altSet.bDescriptorType),
                    desc_obj.class_desc(altSet.bInterfaceClass),
                    desc_obj.class_desc(altSet.bInterfaceSubClass),
                    altSet.bNumEndpoints);
    //     --> Loop Get Device Configuration->Interface->altsetting->endpoint
            for ( int k = 0; k < altSet.bNumEndpoints; k++ ) {
                struct libusb_endpoint_descriptor    endpoint = altSet.endpoint[k];
                printf("        --> Endpoint[%d] : %s --> Attribute : %s -- bEndPointAddress : %u\n", k,
                        desc_obj.descriptor_type_desc(endpoint.bDescriptorType),
                        desc_obj.endpoint_transfer_type_desc(endpoint.bmAttributes & 0x03),
                        endpoint.bEndpointAddress);
            }
        }
    }

    // Release Configuration descriptor libusb_free_config_descriptor(conf_desc);
    libusb_free_config_descriptor(conf_desc);

    // Release the interface libusb_release_interface(handle, interface_num) Loop;
    for ( int i = 0; i < conf_desc->bNumInterfaces; i++ ) {
        libusb_release_interface(handle, i);
    }

}

static int LIBUSB_CALL hotplug_callback_attach(libusb_context *ctx, libusb_device *dev,
           libusb_hotplug_event event, void *user_data)
{
    // Event Callback function when USB device Hot Plugged

    (void) ctx;
    (void) dev;
    (void) event;
    (void) user_data;
    printf("**** hotplug_callback_attach() ****\n");
    event_count++;

    print_usb_info(dev);
    return 0;
}

static int LIBUSB_CALL hotplug_callback_detach(libusb_context *ctx, libusb_device *dev,
           libusb_hotplug_event event, void *user_data)
{
    // Event Callback function when USB device detached

    (void)ctx;
    (void)dev;
    (void)event;
    (void)user_data;
    printf("**** hotplug_callback_detach() ****\n");
    event_count++;

    // Get Device Descriptor and display info Device Detached
    struct libusb_device_descriptor        desc;
    int rc = libusb_get_device_descriptor(dev, &desc);
    if ( rc == LIBUSB_SUCCESS ) {
        printf("Device Detached : %04x:%04x\n", desc.idVendor, desc.idProduct);
    } else {
        printf("Device detached\n");
        printf("Error getting device descriptor: %s\n", libusb_strerror((enum libusb_error)rc));
    }
 
    // If usb device is open, then close libusb_close()
    if (handle) {
        libusb_close(handle);
        handle = NULL;
    }
    return 0;
}

/*
 *  Detect HotPlug USB attach/detach event and desply the device info
 */

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    int rc;

    rc = libusb_init_context(NULL, NULL, 0);   // Initialize libusb
    if (rc != LIBUSB_SUCCESS)
    {
        printf ("failed to initialize libusb: %s\n", libusb_strerror((enum libusb_error)rc));
        return EXIT_FAILURE;
    }

    // 1) Check if platform support HotPlug libusb_has_capability()
    if ( libusb_has_capability( LIBUSB_CAP_HAS_HOTPLUG ) == 0 ) {
        printf("Hotplug capabilities are not supported on this platform\n");
        libusb_exit(NULL);
        return EXIT_FAILURE;
    }
    printf("Hotplug support is available on this platform\n");

    // 2) Register USB Attach event callback libusb_hotplug_register_callback()
     //       LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED
    libusb_hotplug_callback_handle       attach_handle;
    int vendor_id = LIBUSB_HOTPLUG_MATCH_ANY;
    int product_id = LIBUSB_HOTPLUG_MATCH_ANY;
    int class_id   = LIBUSB_HOTPLUG_MATCH_ANY;
    rc = libusb_hotplug_register_callback(NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,
            LIBUSB_HOTPLUG_NO_FLAGS, vendor_id, product_id, class_id,
            hotplug_callback_attach, NULL, &attach_handle);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Error registering attach callback <-- hotplug_callback_attach()\n");
        libusb_exit(NULL);
        return EXIT_FAILURE;
    }
    printf("Registered Event Attach Callback <-- hotplug_callback_attach()\n");

    // 3) Register USB Detach callback libusb_hotplug_register_callback()
    //       LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT
    libusb_hotplug_callback_handle       detach_handle;
    rc = libusb_hotplug_register_callback(NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
            LIBUSB_HOTPLUG_NO_FLAGS, vendor_id, product_id, class_id,
            hotplug_callback_detach, NULL, &detach_handle);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Error registering detach callback <-- hotplug_callback_detach()\n");
        libusb_exit(NULL);
        return EXIT_FAILURE;
    }
    printf("Registered Event Detach Callback <-- hotplug_callback_detach()\n");

    // 4) Start Listening HotPlug USB Device to Attach and Detach
    printf("--> Start Listening Hotplug USB device to be attached/detached\n");
    while ( event_count < 2 ) {
        rc = libusb_handle_events(NULL);
        if ( rc != LIBUSB_SUCCESS ) {
            printf("libusb_handle_events() Failed %s\n", libusb_strerror((enum libusb_error)rc));
        }
    }
 
    // 5) Deregisters a hotplug callback.
    libusb_hotplug_deregister_callback(NULL, attach_handle);
    libusb_hotplug_deregister_callback(NULL, detach_handle);

    libusb_exit(NULL);                        // Deinitialize libusb
    return EXIT_SUCCESS;
}
