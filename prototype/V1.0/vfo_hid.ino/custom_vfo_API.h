#pragma once

#include <Arduino.h>
#include <HID-Settings.h>

typedef union ATTRIBUTE_PACKED {
	uint8_t whole8[0];
	uint16_t whole16[0];
	uint32_t whole32[0];
	struct ATTRIBUTE_PACKED {
		uint16_t buttons:16;
		int16_t smooth_dial:16;
		int8_t coarse_dial:8;
	};
} HID_VFODialReport_Data_t;

class VFODialAPI {
public:
	inline VFODialAPI(void);
	inline void begin(void);
	inline void end(void);
  inline size_t releaseAll(void);

  inline void setButtons(uint16_t val);
  inline void setSmooth(int16_t val);
  inline void setCoarse(int8_t val);

  virtual int send(void) = 0;
//	inline void sendstatus(uint16_t buttons, int16_t sd, int8_t cd);

	// Sending is public in the base class for advanced users.
	//virtual void SendReport(void* data, int length) = 0;

protected:
	uint16_t _button;

	int16_t _smooth_dial;

	int8_t _coarse_dial;

  HID_VFODialReport_Data_t _vfoReport;
};

// Implementation is inline
#include "custom_vfo_API.hpp"
