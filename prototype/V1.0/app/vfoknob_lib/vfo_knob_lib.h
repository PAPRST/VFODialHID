#pragma once

#include <stdint.h>
#include <wchar.h>

#include <hidapi/hidapi.h>

#define HID_REPORTID_VFODIAL 11

#define DISPLAY_CMD 1
#define LED_CMD 2

#define MAX_STR 255

typedef struct {
	uint16_t buttons;
	int16_t smooth;
	int8_t coarse;
} HID_VFO_Report;

hid_device *handle;

hid_device * open_by_id(uint16_t vid, uint16_t pid, const wchar_t * sernum);
void close_hid(hid_device * handle);
HID_VFO_Report * read_report(hid_device * handle);
int8_t send_cmd(hid_device * handle, uint8_t cmd, const uint8_t * data, const uint8_t len);
int8_t send_text(hid_device * handle, uint8_t x, uint8_t y, const uint8_t * data, const uint8_t len);

wchar_t man_string[MAX_STR];
wchar_t prod_string[MAX_STR];
wchar_t sernum_string[MAX_STR];
