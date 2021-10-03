#include <stdio.h> // printf
#include <wchar.h> // wprintf
#include <stdint.h>
#include <stdlib.h>

#include <hidapi/hidapi.h>

#include "vfoknob_lib/vfo_knob_lib.h"

int main(int argc, char * argv[]) {
	uint8_t buf[64] = {0};
	int16_t cnt = 0;

	hid_device * h;
	h = open_by_id(0x2341, 0x8036, NULL);

	while (1) {
		HID_VFO_Report * pReport = read_report(h);
		cnt += pReport->smooth;
		printf("Buttons: %04X - Smooth: %d - Coarse: %d - Counter: %d\n", pReport->buttons, pReport->smooth, pReport->coarse, cnt);
		snprintf(buf, 64, "%d", cnt);
		printf("%s\n", buf);
		int8_t res = send_text(h, 0, 0, buf, 61);
		printf("%d\n", res);
	}
}