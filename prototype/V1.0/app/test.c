#include <stdio.h> // printf
#include <wchar.h> // wprintf
#include <stdint.h>

#include <hidapi/hidapi.h>

#define MAX_STR 255

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	// Initialize the hidapi library
	res = hid_init();

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x2341, 0x8036, NULL);

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	// Read Indexed String 1
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	wprintf(L"Indexed String 1: %s\n", wstr);

	// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
	/*buf[0] = 0x0;
	buf[1] = 0x80;
	res = hid_write(handle, buf, 65);
*/
	uint8_t buffer[64] = {0};
	buffer[0] = 1;
	buffer[1] = 0;
	buffer[2] = 0;

	strcat(&buffer[3], "SigDigger interface");
	res = hid_write(handle, buffer, 64);

	//usleep(500000);

	buffer[0] = 2;
	buffer[1] = 0;
	buffer[2] = 0;
	res = hid_write(handle, buffer, 64);
	// Request state (cmd 0x81). The first byte is the report number (0x0).
	/*buf[0] = 0x0;
	buf[1] = 0x81;
	res = hid_write(handle, buf, 65);*/

	// Read requested state
	res = hid_read(handle, buf, 65);

	// Print out the returned buffer.
	for (i = 0; i < 4; i++)
		printf("buf[%d]: %d\n", i, buf[i]);

	// Close the device
	hid_close(handle);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}