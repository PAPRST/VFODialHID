#include <string.h>
#include <stdlib.h>
#include "vfo_knob_lib.h"

hid_device * open_by_id(uint16_t vid, uint16_t pid, const wchar_t * sernum) {
	int res = 0;
	hid_device * handle;
	wchar_t man_string[MAX_STR];
	wchar_t prod_string[MAX_STR];
	wchar_t sernum_string[MAX_STR];
	res = hid_init();
	printf("HID Init %d \n", res);
	if(res == 0) {
		handle = hid_open(vid, pid, sernum);
		printf("HID open %d \n", handle);
		if(handle) {
			// res = hid_get_manufacturer_string(handle, man_string, MAX_STR);
			// if(res == 0) {
			// 	res = hid_get_product_string(handle, prod_string, MAX_STR);
			// 	if(res == 0) {
			// 		res = hid_get_serial_number_string(handle, sernum_string, MAX_STR);
			// 		if(res == 0)
			// 			return handle;
			// 		else
			// 			return 0;
			// 	}
			// }
			return handle;
		}
	}
	return 0;
}

void close_hid(hid_device * handle) {
	hid_close(handle);
	hid_exit();
}

HID_VFO_Report * read_report(hid_device * handle) {
	// Device is using a Multi Report system we need one more uint8_t
	uint8_t report[6];
	uint8_t res = 0;
	uint8_t i = 0;
	printf("Read report\n");
	while(res < 6) {
		res = hid_read(handle, &report[i], 6);
		printf("res: %d", res);
		i += res;
	}
	if(report[0] == HID_REPORTID_VFODIAL) {
		HID_VFO_Report *pReport = (HID_VFO_Report *)malloc(sizeof(HID_VFO_Report));
		memcpy(pReport, &report[1], 5);
		return pReport;
	} else {
		return 0;
	}
}

int8_t send_cmd(hid_device * handle, uint8_t cmd, const uint8_t * data, const uint8_t len) {
	uint8_t * extdata = (uint8_t *) malloc(len+1);
	extdata[0] = cmd;
	memcpy(&extdata[1], data, len);
	uint8_t res = hid_write(handle, extdata, len+1);
	free(extdata);
	/*if(res != len+1) {
		return -1;
	}*/
	return res;
}

int8_t send_text(hid_device * handle, uint8_t x, uint8_t y, const uint8_t * data, const uint8_t len) {
	uint8_t * dispdata = (uint8_t *) malloc(len+2);
	dispdata[0] = x;
	dispdata[1] = y;
	memcpy(&dispdata[2], data, len);
	uint8_t res = send_cmd(handle, DISPLAY_CMD, dispdata, len+2);
	free(dispdata);
	return res;
}

int8_t send_leds(hid_device * handle, const uint16_t leds) {
	uint8_t * data = (uint8_t *) malloc(63);
	data[0] = leds >> 8;
	data[1] = leds & 0x00FF;
	uint8_t res = send_cmd(handle, LED_CMD, data, 63);
	free(data);
	return res;
}