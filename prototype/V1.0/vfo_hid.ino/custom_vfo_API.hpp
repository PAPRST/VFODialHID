#pragma once

VFODialAPI::VFODialAPI(void) {
	_button = 0;
	_smooth_dial = 0;
	_coarse_dial = 0;
}

void VFODialAPI::begin(void) {
	send();
}

void VFODialAPI::setButtons(uint16_t val) {
  _vfoReport.buttons = val;
}

void VFODialAPI::setSmooth(int16_t val) {
  _vfoReport.smooth_dial = val;
}
void VFODialAPI::setCoarse(int8_t val) {
  _vfoReport.coarse_dial = val;
}

//void VFODialAPI::end(void) {
//	_button = 0;
//	_smooth_dial = 0;
//	_coarse_dial = 0;
//	//sendstatus(0, 0, 0);
//}

//void VFODialAPI::sendstatus(uint16_t buttons, int16_t sd, int8_t cd) {
//	HID_VFODialReport_Data_t report;
//
//	report.buttons = buttons;
//	report.smooth_dial = sd;
//	report.coarse_dial = cd;
//
//	SendReport(&report, sizeof(report));
//}
