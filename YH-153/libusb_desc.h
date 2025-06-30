#ifndef LIBUSB_DESC_H
#define LIBUSB_DESC_H

#include <stdlib.h>
#include <libusb-1.0/libusb.h>

// https://www.usb.org/sites/default/files/usbmassbulk_10.pdf
// Section 5.1: Command Block Wrapper (CBW)
struct command_block_wrapper {
    uint8_t dCBWSignature[4];
    uint32_t dCBWTag;
    uint32_t dCBWDataTransferLength;
    uint8_t bmCBWFlags;
    uint8_t bCBWLUN;
    uint8_t bCBWCBLength;
    uint8_t CBWCB[16];
};

// Section 5.2: Command Status Wrapper (CSW)
struct command_status_wrapper {
    uint8_t dCSWSignature[4];
    uint32_t dCSWTag;
    uint32_t dCSWDataResidue;
    uint8_t bCSWStatus;
};

class Libusb_Desc {
public:
    Libusb_Desc();
    const char * speed_desc(int speed_code);
    const char * class_desc(uint8_t class_code);
    const char * descriptor_type_desc(uint8_t type_code);
    const char * bcdUSB_desc(uint16_t bcdUSB_code);
    const char * transfer_type_desc(uint8_t transfer_code);
    const char * endpoint_transfer_type_desc(uint8_t transfer_code);
    ~Libusb_Desc();
private:
    uint8_t code;
};

#endif
