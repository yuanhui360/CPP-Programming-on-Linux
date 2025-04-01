#ifndef LIBUSB_DESC_H
#define LIBUSB_DESC_H

#include <stdlib.h>
#include <libusb-1.0/libusb.h>

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
