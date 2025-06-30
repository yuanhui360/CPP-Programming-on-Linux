#include <iostream>
#include <sstream>      // std::stringstream
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libusb-1.0/libusb.h>
#include "libusb_desc.h"

#define REQUEST_SENSE_LENGTH          0x12
#define INQUIRY_LENGTH                0x24
#define BOMS_RESET                    0xFF
#define BOMS_GET_MAX_LUN              0xFE

static uint32_t tag = 1;
int event_count = 0;
libusb_device_handle *handle = NULL;

void LIBUSB_CALL display_buffer_hex(unsigned char *buffer, unsigned int size)
{
    unsigned int i, j, k;

    for (i=0; i<size; i+=16) {
        printf("\n  %08x  ", i);
        for(j=0,k=0; k<16; j++,k++) {
            if (i+j < size) {
                printf("%02x", buffer[i+j]);
            } else {
                printf("  ");
            }
            printf(" ");
        }
        printf(" ");
        for(j=0,k=0; k<16; j++,k++) {
            if (i+j < size) {
                if ((buffer[i+j] < 32) || (buffer[i+j] > 126)) {
                    printf(".");
                } else {
                    printf("%c", buffer[i+j]);
                }
            }
        }
    }
    printf("\n" );
}

void LIBUSB_CALL test_mass_storage(libusb_device *dev, uint8_t endpoint_in, uint8_t endpoint_out)
{
    (void) dev;
    (void) endpoint_in;
    (void) endpoint_out;

    int rc;
    int iface = 0;       // default interface number for USB device

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

    // TODO:
    // 1) claim interface
    //    1-1) libusb_set_auto_detach_kernel_driver()
    rc = libusb_set_auto_detach_kernel_driver(handle, 1);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("INFO: libusb_set_auto_detach_kernel_driver() not Support : %s\n", libusb_strerror((enum libusb_error)rc));
    }

    //    1-2) libusb_claim_interface()
    rc = libusb_claim_interface(handle, iface);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Error libusb_claim_interface() : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }

    // 2) check if a kernel driver active from an interface.
    //    2-1) check libusb_kernel_driver_active()
    //    2-2) if active, libusb_detach_kernel_driver()
    rc = libusb_kernel_driver_active(handle, iface);
    if ( rc == 1 ) {
        printf("INFO: A Kernel Driver Active from Interface(%d) \n", iface);
        rc = libusb_detach_kernel_driver(handle, iface);
        if ( rc != LIBUSB_SUCCESS ) {
            printf("Error libusb_detach_kernel_driver() : %s\n", libusb_strerror((enum libusb_error)rc));
            return;
        }
    } else if ( rc != 0 ) {
        printf("Error libusb_kernel_driver_active() : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }

    // 3) Get Max LUN <-- Logical Unit Number 1 byte, 0-15
    /*
     *  To issue a Get Max LUN device request, the host shall issue a device reque
     *  set on the default pipe of:
     *      • bmRequestType: Class, Interface, device to host
     *      • bRequest field set to 254 (FEh)
     *      • wValue field set to 0
     *      • wIndex field set to the interface number
     *      • wLength field set to 1
     *      • Data field 1 byte
     *
     */

    uint8_t lun;
    rc = libusb_control_transfer(handle,
            LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_IN,
            0xFE, 0, iface, &lun, 1, 1000);
    if ( rc == LIBUSB_ERROR_PIPE ) {
        lun = 0;
    } else if ( rc < 0 ) {
        printf("Error Get Max LUN request : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }
    printf("Reading Max LUN (Logical Unit Number) : %d\n", lun);

    // 4) SCSI Inquiry Command (0x12) Example
    //    4-1) Prepare Command
    uint8_t cdb[16];         // SCSI Command Descriptor Block 6, 10, 12, 16 bytes
    uint8_t buffer[252];
    struct command_block_wrapper  cbw;
    struct command_status_wrapper csw;

    memset(buffer, '\0', sizeof(buffer));
    memset(cdb,    '\0', sizeof(cdb));
    memset(&cbw,   '\0', sizeof(cbw));
    memset(&csw,   '\0', sizeof(csw));

    cdb[0] = 0x12;                  // SCSI Inquiry command code 0x12
    cdb[4] = INQUIRY_LENGTH;        // at leaset 36 bytes 0x24

    cbw.dCBWSignature[0]  = 'U';    // 0x55
    cbw.dCBWSignature[1]  = 'S';    // 0x53
    cbw.dCBWSignature[2]  = 'B';    // 0x42
    cbw.dCBWSignature[3]  = 'C';    // 0x43

    cbw.dCBWTag = tag++;
    cbw.dCBWDataTransferLength = INQUIRY_LENGTH;
    cbw.bmCBWFlags = LIBUSB_ENDPOINT_IN;
    cbw.bCBWLUN  = lun;
    cbw.bCBWCBLength  = 16;
    memcpy(cbw.CBWCB, cdb, 16);

    //    4-2) Send Command to USB Flush Drive (Mass Storage) Device
    int trfd_bytes;
    rc = libusb_bulk_transfer(handle, endpoint_out, (unsigned char *)&cbw, 31, &trfd_bytes, 1000);
    if ( rc == LIBUSB_ERROR_PIPE ) {
        libusb_clear_halt(handle, endpoint_out);
        rc = libusb_bulk_transfer(handle, endpoint_out, (unsigned char *)&cbw, 31, &trfd_bytes, 1000);
    }
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Error sending SCSI Inquiry Command : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }
    printf("Sending SCSI Inquiry Command (0x12) : %d bytes \n", trfd_bytes);

    //    4-3) Read Inquiry Command data from Flush Drive (Mass Storage)
    int rcvd_bytes;
    rc = libusb_bulk_transfer(handle, endpoint_in, (unsigned char *)&buffer, INQUIRY_LENGTH, &rcvd_bytes, 1000);
    if ( rc == LIBUSB_ERROR_PIPE ) {
        libusb_clear_halt(handle, endpoint_in);
        rc = libusb_bulk_transfer(handle, endpoint_in, (unsigned char *)&buffer, INQUIRY_LENGTH, &rcvd_bytes, 1000);
    }
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Error Receiving SCSI Inquiry Command data : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }
    printf("SCSI Inquiry Command (0x12) Success Received : %d bytes \n", rcvd_bytes);
    display_buffer_hex((unsigned char *)&buffer, rcvd_bytes);

    //    4-4) Check SCSI Command Status if ready for sending next command
    memset(&csw,   '\0', sizeof(csw));
    trfd_bytes = 0;
    rc = libusb_bulk_transfer(handle, endpoint_in, (unsigned char *)&csw, 13, &trfd_bytes, 1000);
    if ( rc == LIBUSB_ERROR_PIPE ) {
        libusb_clear_halt(handle, endpoint_in);
        rc = libusb_bulk_transfer(handle, endpoint_in, (unsigned char *)&csw, 13, &trfd_bytes, 1000);
    }
    printf("USB Mass Storage Command Status Check : %02X, (%s)\n", csw.bCSWStatus, csw.bCSWStatus?"FAILED":"Success");

    // 5) REQUEST SENSE command (0x03) Example
    //    5-1) Prepare Command

    memset(buffer, '\0', sizeof(buffer));
    memset(cdb,    '\0', sizeof(cdb));
    memset(&cbw,   '\0', sizeof(cbw));
    memset(&csw,   '\0', sizeof(csw));

    cdb[0] = 0x03;                  // SCSI REQUEST SENSE command code 0x03
    cdb[4] = REQUEST_SENSE_LENGTH;  // at leaset 18 bytes 0x12

    cbw.dCBWSignature[0]  = 'U';    // 0x55
    cbw.dCBWSignature[1]  = 'S';    // 0x53
    cbw.dCBWSignature[2]  = 'B';    // 0x42
    cbw.dCBWSignature[3]  = 'C';    // 0x43

    cbw.dCBWTag = tag++;
    cbw.dCBWDataTransferLength = REQUEST_SENSE_LENGTH;
    cbw.bmCBWFlags = LIBUSB_ENDPOINT_IN;
    cbw.bCBWLUN  = lun;
    cbw.bCBWCBLength  = 16;
    memcpy(cbw.CBWCB, cdb, 16);

    //    5-2) Send REQUEST SENSE command 0x03 to Flush Drive (Mass Storage)
    trfd_bytes = 0;
    rc = libusb_bulk_transfer(handle, endpoint_out, (unsigned char *)&cbw, 31, &trfd_bytes, 1000);
    if ( rc == LIBUSB_ERROR_PIPE ) {
        libusb_clear_halt(handle, endpoint_out);
        rc = libusb_bulk_transfer(handle, endpoint_out, (unsigned char *)&cbw, 31, &trfd_bytes, 1000);
    }
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Error sending SCSI REQUEST SENSE Command : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }
    printf("Sending SCSI REQUEST SENSE Command (0x03) : %d bytes \n", trfd_bytes);

    //    5-3) Read REQUEST SENSE Command data from Flush Drive (Mass Storage)
    rcvd_bytes = 0;
    rc = libusb_bulk_transfer(handle, endpoint_in, (unsigned char *)&buffer, REQUEST_SENSE_LENGTH, &rcvd_bytes, 1000);
    if ( rc == LIBUSB_ERROR_PIPE ) {
        libusb_clear_halt(handle, endpoint_in);
        rc = libusb_bulk_transfer(handle, endpoint_in, (unsigned char *)&buffer, REQUEST_SENSE_LENGTH, &rcvd_bytes, 1000);
    }
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Error Receiving SCSI REQUEST SENSE Command data : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }
    printf("SCSI REQUEST SENSE Command (0x03) Success Received : %d bytes \n", rcvd_bytes);
    display_buffer_hex((unsigned char *)&buffer, rcvd_bytes);

    //    5-4) Check SCSI Command Status if ready for sending next command
    memset(&csw,   '\0', sizeof(csw));
    trfd_bytes = 0;
    rc = libusb_bulk_transfer(handle, endpoint_in, (unsigned char *)&csw, 13, &trfd_bytes, 1000);
    if ( rc == LIBUSB_ERROR_PIPE ) {
        libusb_clear_halt(handle, endpoint_in);
        rc = libusb_bulk_transfer(handle, endpoint_in, (unsigned char *)&csw, 13, &trfd_bytes, 1000);
    }
    printf("USB Mass Storage Command Status Check : %02X, (%s)\n", csw.bCSWStatus, csw.bCSWStatus?"FAILED":"Success");

    libusb_release_interface(handle, iface);
}

void LIBUSB_CALL print_usb_info(libusb_device *dev)
{
    (void) dev;

    // Get Device descriptor libusb_get_device_descriptor()
    Libusb_Desc        desc_obj;
    struct libusb_device_descriptor     desc;
    int rc = libusb_get_device_descriptor(dev, &desc);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("Error getting device descriptor : %s\n", libusb_strerror((enum libusb_error)rc));
        return;
    }

    // Display hotplug SUB device descriptor level info
    printf("Device Found : VID:PID [%04x:%04x] -- %s --> Number Configuration : %u\n",
            desc.idVendor, desc.idProduct, desc_obj.descriptor_type_desc(desc.bDescriptorType),
            desc.bNumConfigurations);
    printf("  <%s/%s> %s %s \n",
            desc_obj.class_desc(desc.bDeviceClass),
            desc_obj.class_desc(desc.bDeviceSubClass),
            desc_obj.bcdUSB_desc(desc.bcdUSB),
            desc_obj.speed_desc(libusb_get_device_speed(dev)));

    // Get Device (current) Configuration level parameters
    struct libusb_config_descriptor     *conf_desc;
    rc = libusb_get_active_config_descriptor(dev, &conf_desc);
    if ( rc != LIBUSB_SUCCESS ) {
        printf("  Failed to get current configuration descriptor\n");
        return;
    }
    printf("  --> %s --> Number Interfaces: %u\n", desc_obj.descriptor_type_desc(conf_desc->bDescriptorType),
            conf_desc->bNumInterfaces);

    uint8_t endpoint_in = 0, endpoint_out = 0;  // default IN and OUT endpoints
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
                printf("        --> Endpoint[%d] : %s --> Attribute : %s -- bEndPointAddress : %04u  ", k,
                        desc_obj.descriptor_type_desc(endpoint.bDescriptorType),
                        desc_obj.endpoint_transfer_type_desc(endpoint.bmAttributes & LIBUSB_TRANSFER_TYPE_MASK),
                        endpoint.bEndpointAddress);
                if ((endpoint.bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) &
                    (LIBUSB_TRANSFER_TYPE_BULK | LIBUSB_TRANSFER_TYPE_INTERRUPT)) {
                    if (endpoint.bEndpointAddress & LIBUSB_ENDPOINT_IN) {
                        if (!endpoint_in)
                            endpoint_in = endpoint.bEndpointAddress;
                        printf(" endpoint_in\n");
                    } else {
                        if (!endpoint_out)
                            endpoint_out = endpoint.bEndpointAddress;
                        printf(" endpoint_out\n");
                    }
                }
            }
        }
    }

    test_mass_storage(dev, endpoint_in, endpoint_out);

    // Release Configuration descriptor libusb_free_config_descriptor(conf_desc);
    libusb_free_config_descriptor(conf_desc);

    // Release the interface libusb_release_interface(handle, interface_num) Loop;
    for ( int i = 0; i < conf_desc->bNumInterfaces; i++ ) {
        libusb_release_interface(handle, i);
    }

}

/*
 *  Program will have 2 argument from command line, VID and PID
 *  Then : Find if device exist, if Yes, print device parameters
 *  and performing I/O communication between host and USB devices
 *
 *  The device using in this example is USB Flush Drive, USB Mass
 *  Storage class device
 */

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::stringstream       str_VID;
    std::stringstream       str_PID;
    uint16_t vendor_id;
    uint16_t product_id;
    int rc;
    handle = NULL;
    tag = 1;

    if ( argc != 3 ) {
        printf("usage: %s <VID> <PID>\n", argv[0]);
        printf("Example : %s 04e7 \"8001\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    // string -> uint16_t (argv[1] -> vendor_id, argv[2] -> product_id)

    str_VID << argv[1];
    str_VID >> std::hex >> vendor_id;
    str_PID << argv[2];
    str_PID >> std::hex >> product_id;
    printf("Usage: %s VID:PID [%04x:%04x] \n", argv[0], vendor_id, product_id);

    rc = libusb_init_context(NULL, NULL, 0);   // Initialize libusb
    if (rc != LIBUSB_SUCCESS)
    {
        printf ("failed to initialize libusb: %s\n", libusb_strerror((enum libusb_error)rc));
        return EXIT_FAILURE;
    }

    libusb_device **devs;   // <-- Devices array
    libusb_device *dev;
    ssize_t cnt = libusb_get_device_list(NULL, &devs); // Get list of connected USB devices
    if (cnt < 0){
        libusb_exit(NULL);
        return (int) cnt;
    }

    struct libusb_device_descriptor     dev_desc;
    for ( int i = 0; devs[i] != NULL; i++) {
        dev = devs[i];
        int rc = libusb_get_device_descriptor(devs[i], &dev_desc);
        if ( rc != LIBUSB_SUCCESS ) {
            continue;
        }
        if ( dev_desc.idVendor == vendor_id && dev_desc.idProduct == product_id ) {
            // Found Device
            print_usb_info(dev);
        }
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);                        // Deinitialize libusb
    return EXIT_SUCCESS;
}
