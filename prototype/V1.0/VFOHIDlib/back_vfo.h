#pragma once

#include <Arduino.h>
#include <HID.h>
#include <HID-Settings.h>
#include "custom_vfo_API.h"

#define HID_REPORTID_VFODIAL 11

class VFODial_ : public VFODialAPI {
public:
	VFODial_(void);
protected:
	virtual inline void SendReport(void* data, int length) override;
};

extern VFODial_ VFODial;
