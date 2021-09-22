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

int VFO_::getInterface(uint8_t* interfaceCount)
{
	*interfaceCount += 1; // uses 1
	HIDDescriptor hidInterface = {
		D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_NONE, HID_PROTOCOL_NONE),
		D_HIDREPORT(sizeof(_hidMultiReportDescriptorVFODial)),
		D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01)
	};
	return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

VFO_::VFO_(void) : PluggableUSBModule(1, 1, epType), protocol(HID_REPORT_PROTOCOL), idle(1), featureReport(NULL), featureLength(0)
{
	epType[0] = EP_TYPE_INTERRUPT_IN;
	PluggableUSB().plug(this);
}

int VFO_::getDescriptor(USBSetup& setup)
{
	// Check if this is a HID Class Descriptor request
	if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) { return 0; }
	if (setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE) { return 0; }

	// In a HID Class Descriptor wIndex cointains the interface number
	if (setup.wIndex != pluggedInterface) { return 0; }

	// Reset the protocol on reenumeration. Normally the host should not assume the state of the protocol
	// due to the USB specs, but Windows and Linux just assumes its in report mode.
	protocol = HID_REPORT_PROTOCOL;

	return USB_SendControl(TRANSFER_PGM, _hidMultiReportDescriptorVFODial, sizeof(_hidMultiReportDescriptorVFODial));
}

bool VFO_::setup(USBSetup& setup)
{
  
	if (pluggedInterface != setup.wIndex) {
		return false;
	}

	uint8_t request = setup.bRequest;
	uint8_t requestType = setup.bmRequestType;

	if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE)
	{
		if (request == HID_GET_REPORT) {
			// TODO: HID_GetReport();
			return true;
		}
		if (request == HID_GET_PROTOCOL) {
			// TODO improve
#ifdef __AVR__
			UEDATX = protocol;
#endif
			return true;
		}
		if (request == HID_GET_IDLE) {
			// TODO improve
#ifdef __AVR__
			UEDATX = idle;
#endif
			return true;
		}
	}

	if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE)
	{
		if (request == HID_SET_PROTOCOL) {
			protocol = setup.wValueL;
			return true;
		}
		if (request == HID_SET_IDLE) {
			idle = setup.wValueL;
			return true;
		}
		if (request == HID_SET_REPORT)
		{
    
			// Check if data has the correct length afterwards
			int length = setup.wLength;

			// Feature (set feature report)
			if(setup.wValueH == HID_REPORT_TYPE_FEATURE){
				// No need to check for negative featureLength values,
				// except the host tries to send more then 32k bytes.
				// We dont have that much ram anyways.
				if (length == featureLength) {
					USB_RecvControl(featureReport, featureLength);

					// Block until data is read (make length negative)
					disableFeatureReport();
					return true;
				}
				// TODO fake clear data?
			}

			// Output (set led states)
			else if(setup.wValueH == HID_REPORT_TYPE_OUTPUT){
				if(length == sizeof(text)){
					USB_RecvControl(text, length);
          dataRcvd = 1;
					return true;
				}
			}

			// Input (set HID report)
			else if(setup.wValueH == HID_REPORT_TYPE_INPUT)
			{
				if(length == sizeof(_vfoReport)){
					USB_RecvControl(&_vfoReport, length);
					return true;
				}
			}
		}
	}

	return false;
}

uint8_t VFO_::getProtocol(void){
    return protocol;
}

int VFO_::send(void){
  return USB_Send(pluggedEndpoint | TRANSFER_RELEASE, &_vfoReport, sizeof(_vfoReport));
}

int VFO_::sendReport(void) {
  uint8_t id = HID_REPORTID_VFODIAL;
  auto ret = USB_Send(pluggedEndpoint, &id, 1);
  if (ret < 0) return ret;
  auto ret2 = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, &_vfoReport, sizeof(_vfoReport));
  if (ret2 < 0) return ret2;
  return ret + ret2;

}

void VFO_::wakeupHost(void){
#ifdef __AVR__
  USBDevice.wakeupHost();
#endif
}

VFO_ VFO;
