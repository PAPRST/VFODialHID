#pragma once

#include <Arduino.h>
#include "HID.h"
#include "HID-Settings.h"
#include "custom_vfo_API.h"

#define HID_REPORTID_VFODIAL 11

#define DISPLAY_CMD 1
#define LED_CMD 2

class VFO_ : public PluggableUSBModule, public VFODialAPI {
	public:
		VFO_(void);
		uint8_t * getText(void);
		uint8_t getProtocol(void);
		void wakeupHost(void);

		void setFeatureReport(void* report, int length){
	        if(length > 0){
	            featureReport = (uint8_t*)report;
	            featureLength = length;
	            
	            // Disable feature report by default
	            disableFeatureReport();
	        }
	    }

	    int availableFeatureReport(void){
	        if(featureLength < 0){
	            return featureLength & ~0x8000;
	        }
	        return 0;
	    }
    
	    void enableFeatureReport(void){
	        featureLength &= ~0x8000;
	    }
	    
	    void disableFeatureReport(void){
	        featureLength |= 0x8000;
	    }

     uint8_t isDataRdy(void) {
      return dataRcvd;
     }

     void rstDataRdy(void) {
      dataRcvd = 0;
     }

     uint8_t * getTxt(void) {
      return text;
     }

     virtual int send(void) final;
     virtual int sendReport(void) final;

	protected:
  		int getInterface(uint8_t * interfaceCount);
  		int getDescriptor(USBSetup & setup);
  		bool setup(USBSetup & setup);

  		uint8_t text[64];
      uint16_t leds = 0;
		  EPTYPE_DESCRIPTOR_SIZE epType[1];
    	uint8_t protocol;
    	uint8_t idle;

    	uint8_t* featureReport;
    	int featureLength;

      uint8_t dataRcvd = 0;

};
extern VFO_ VFO;
