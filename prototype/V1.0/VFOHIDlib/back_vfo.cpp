#include "vfo.h"

static const uint8_t _hidMultiReportDescriptorVFODial[] PROGMEM = {
    // Integrated Radial Controller TLC
    0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
    0x09, 0x08,       // USAGE (Multi-Axis Controller)
    0xa1, 0x01,       // COLLECTION (Application)
    0x85, HID_REPORTID_VFODIAL,       //   REPORT_ID (Custom ID)
    0x05, 0x0d,       //   USAGE_PAGE (Digitizers)
    0x09, 0x21,       //   USAGE (Puck)
    0xa1, 0x00,       //   COLLECTION (Physical)
    
    0x05, 0x09,       //     USAGE_PAGE (Buttons)
    0x09, 0x01,       //     USAGE (Button 1)
    0x95, 0x01,       //     REPORT_COUNT (1)
    0x75, 0x10,       //     REPORT_SIZE (16)
    0x15, 0x00,       //     LOGICAL_MINIMUM (0)
    0x25, 0x01,       //     LOGICAL_MAXIMUM (1)
    0x81, 0x02,       //     INPUT (Data,Var,Abs)
    
    0x05, 0x01,       //     USAGE_PAGE (Generic Desktop)
    0x09, 0x37,       //     USAGE (Dial)
    0x95, 0x01,       //     REPORT_COUNT (1)
    0x75, 0x10,       //     REPORT_SIZE (16)
    0x16, 0xf0, 0xf1, //     LOGICAL_MINIMUM (-3600)
    0x26, 0x10, 0x0e, //     LOGICAL_MAXIMUM (3600)
    0x81, 0x06,       //     INPUT (Data,Var,Rel)

    0x05, 0x01,       //     USAGE_PAGE (Generic Desktop)
    0x09, 0x37,       //     USAGE (Dial)
    0x95, 0x01,       //     REPORT_COUNT (1)
    0x75, 0x08,       //     REPORT_SIZE (8)
    0x15, 0x81,       //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,       //     LOGICAL_MAXIMUM (127)
    0x81, 0x06,       //     INPUT (Data,Var,Rel)
    
    0xc0,             //   END_COLLECTION
    0xc0              // END_COLLECTION
};

VFODial_::VFODial_(void)
{
    static HIDSubDescriptor node(_hidMultiReportDescriptorVFODial, sizeof(_hidMultiReportDescriptorVFODial));
    HID().AppendDescriptor(&node);
}

void VFODial_::SendReport(void *data, int length)
{
    HID().SendReport(HID_REPORTID_VFODIAL, data, length);
}

VFODial_ VFODial;
