#include "libusb_desc.h"
#include <stdio.h>

Libusb_Desc::Libusb_Desc() {
    code = 0;
}

const char * Libusb_Desc::speed_desc(int speed_code) {

    const char *speed_desc;

    switch (speed_code) {
       case LIBUSB_SPEED_LOW:        speed_desc = "LOW SPEED LS (1.5M)"; break;   // low speed (1.5MBit/s
       case LIBUSB_SPEED_FULL:       speed_desc = "FULL SPEED FS (12M)"; break;    // full speed (12MBit/
       case LIBUSB_SPEED_HIGH:       speed_desc = "HIGH SPEED HS (480M)"; break;   // high speed (480MBit
       case LIBUSB_SPEED_SUPER:      speed_desc = "SUPER SPEED SS (5G)"; break;     // super speed (5GBit
       case LIBUSB_SPEED_SUPER_PLUS: speed_desc = "SUPER PLUS SPEED SS+ (10G)"; break;    // super speed
       default:                      speed_desc = "Unknown Speed";
    }
    return speed_desc;
}

const char * Libusb_Desc::class_desc(uint8_t dev_class) {

        const char *class_ptr = NULL;

        switch(dev_class) {
          case LIBUSB_CLASS_PER_INTERFACE:       class_ptr = "Independent Device"; break;
          case LIBUSB_CLASS_AUDIO:               class_ptr = "Audio Device"; break;
          case LIBUSB_CLASS_PER_INTERFACE:       class_ptr = "Independent Device"; break;
          case LIBUSB_CLASS_AUDIO:               class_ptr = "Audio Device"; break;
          case LIBUSB_CLASS_COMM:                class_ptr = "Communication Device"; break;
          case LIBUSB_CLASS_HID:                 class_ptr = "Human Interface Device"; break;
          case LIBUSB_CLASS_PHYSICAL:            class_ptr = "Physical Device"; break;
          case LIBUSB_CLASS_IMAGE:               class_ptr = "Image Device"; break;
          case LIBUSB_CLASS_PRINTER:             class_ptr = "Printer Device"; break;
          case LIBUSB_CLASS_MASS_STORAGE:        class_ptr = "Mass Storage Device"; break;
          case LIBUSB_CLASS_HUB:                 class_ptr = "Hub Device"; break;
          case LIBUSB_CLASS_DATA:                class_ptr = "Data Device"; break;
          case LIBUSB_CLASS_SMART_CARD:          class_ptr = "Smart Card Device"; break;
          case LIBUSB_CLASS_CONTENT_SECURITY:    class_ptr = "Content Security Device"; break;
          case LIBUSB_CLASS_VIDEO:               class_ptr = "Video Device"; break;
          case LIBUSB_CLASS_PERSONAL_HEALTHCARE: class_ptr = "Personal Healthcare Device"; break;
          case LIBUSB_CLASS_DIAGNOSTIC_DEVICE:   class_ptr = "Diagnostic Device"; break;
          case LIBUSB_CLASS_WIRELESS:            class_ptr = "Wireless Device"; break;
          case LIBUSB_CLASS_MISCELLANEOUS:       class_ptr = "Miscellaneous Device"; break;
          case LIBUSB_CLASS_APPLICATION:         class_ptr = "Application Device"; break;
          case LIBUSB_CLASS_VENDOR_SPEC:         class_ptr = "Vendor Specific Device"; break;
          default:                               class_ptr = "Unknown Class";
        }
    return class_ptr;
}

const char * Libusb_Desc::descriptor_type_desc(uint8_t desc_type) {
        const char *type_ptr = NULL;
        switch(desc_type) {
          case LIBUSB_DT_DEVICE:                 type_ptr = "Device Descriptor."; break;
          case LIBUSB_DT_CONFIG:                 type_ptr = "Configuration Descriptor."; break;
          case LIBUSB_DT_CONFIG:                 type_ptr = "Configuration Descriptor."; break;
          case LIBUSB_DT_STRING:                 type_ptr = "String Descriptor."; break;
          case LIBUSB_DT_INTERFACE:              type_ptr = "Interface Descriptor."; break;
          case LIBUSB_DT_ENDPOINT:               type_ptr = "Endpoint Descriptor."; break;
          case LIBUSB_DT_INTERFACE_ASSOCIATION:  type_ptr = "Interface Association Descriptor."; break;
          case LIBUSB_DT_BOS:                    type_ptr = "BOS descriptor."; break;
          case LIBUSB_DT_DEVICE_CAPABILITY:      type_ptr = "Device Capability descriptor."; break;
          case LIBUSB_DT_HID:                    type_ptr = "HID descriptor."; break;
          case LIBUSB_DT_REPORT:                 type_ptr = "HID report descriptor."; break;
          case LIBUSB_DT_PHYSICAL:               type_ptr = "Physical descriptor."; break;
          case LIBUSB_DT_HUB:                    type_ptr = "Hub descriptor."; break;
          case LIBUSB_DT_SUPERSPEED_HUB:         type_ptr = "SuperSpeed Hub descriptor."; break;
          case LIBUSB_DT_SS_ENDPOINT_COMPANION:  type_ptr = "SuperSpeed Endpoint Companion descriptor.";
          default:                               type_ptr = "Unknown Descriptor Type";
        }
        return type_ptr;
}

const char * Libusb_Desc::bcdUSB_desc(uint16_t bcdUSB_code) {
        const char *type_ptr = NULL;
        switch(bcdUSB_code) {
          case 0x0100: type_ptr = "USB 1.0."; break;
          case 0x0110: type_ptr = "USB 1.1."; break;
          case 0x0200: type_ptr = "USB 2.0."; break;
          case 0x0300: type_ptr = "USB 3.0."; break;
          case 0x0310: type_ptr = "USB 3.1."; break;
          case 0x0320: type_ptr = "USB 3.2."; break;
          case 0x0400: type_ptr = "USB 4.0."; break;
          default:     type_ptr = "Unknown USB Type";
        }
        return type_ptr;
}


const char * Libusb_Desc::transfer_type_desc(uint8_t transfer_type) {
        const char *type_ptr = NULL;
        switch(transfer_type) {
          case LIBUSB_TRANSFER_TYPE_CONTROL:     type_ptr = "Control transfer."; break;
          case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS: type_ptr = "Isochronous transfer."; break;
          case LIBUSB_TRANSFER_TYPE_BULK:        type_ptr = "Bulk transfer."; break;
          case LIBUSB_TRANSFER_TYPE_INTERRUPT:   type_ptr = "Interrupt transfer."; break;
          case LIBUSB_TRANSFER_TYPE_BULK_STREAM: type_ptr = "Bulk stream transfer."; break;
          default:     type_ptr = "Unknown Transfer Type";
        }
        return type_ptr;
}

const char * Libusb_Desc::endpoint_transfer_type_desc(uint8_t transfer_type) {
        const char *type_ptr = NULL;
        switch(transfer_type) {
          case LIBUSB_ENDPOINT_TRANSFER_TYPE_CONTROL:     type_ptr = "Control endpoint."; break;
          case LIBUSB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS: type_ptr = "Isochronous endpoint."; break;
          case LIBUSB_ENDPOINT_TRANSFER_TYPE_BULK:        type_ptr = "Bulk endpoint."; break;
          case LIBUSB_ENDPOINT_TRANSFER_TYPE_INTERRUPT:   type_ptr = "Interrupt endpoint."; break;
          default:     type_ptr = "Unknown Endpoint Transfer Type";
        }
        return type_ptr;
}

Libusb_Desc::~Libusb_Desc() {
}
