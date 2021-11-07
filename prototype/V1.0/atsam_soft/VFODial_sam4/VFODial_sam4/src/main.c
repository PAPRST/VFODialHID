/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "main.h"

void init_screen(void) {

}

void init_tc(void) {
	
}

void init_dial(void) {
	// GPIO Init
	ioport_init();
	
	/* Initialize RGB LED, turned off */
	ioport_set_pin_dir(RLED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(RLED, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_dir(GLED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(GLED, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(BLED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(BLED, IOPORT_PIN_LEVEL_LOW);
	
	/* Initialize Switch LED */
	ioport_set_pin_dir(PB1_LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PB1_LED, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PB2_LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PB2_LED, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(PB3_LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PB3_LED, IOPORT_PIN_LEVEL_HIGH);
	
	ioport_set_pin_dir(LEFT_LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LEFT_LED, IOPORT_PIN_LEVEL_HIGH);
	
	ioport_set_pin_dir(RIGHT_LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(RIGHT_LED, IOPORT_PIN_LEVEL_HIGH);
	
	/* Initialize Coder Input */
	ioport_set_pin_dir(SM_COD_N, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SM_COD_P, IOPORT_DIR_INPUT);
	
	ioport_set_pin_mode(SM_COD_N, IOPORT_MODE_PULLUP | IOPORT_MODE_GLITCH_FILTER);
	ioport_set_pin_sense_mode(SM_COD_N, IOPORT_SENSE_BOTHEDGES);
	
	ioport_set_pin_mode(SM_COD_P, IOPORT_MODE_PULLUP | IOPORT_MODE_GLITCH_FILTER);
	ioport_set_pin_sense_mode(SM_COD_P, IOPORT_SENSE_BOTHEDGES);
	
	if (!gpio_set_pin_callback(SM_COD_N, coder_detect, 1) || !gpio_set_pin_callback(SM_COD_P, coder_detect, 1)) {
		printf("Set pin callback failure!\r\n");
		while (1) {
		}
	}
	
	gpio_enable_pin_interrupt(SM_COD_N);
	gpio_enable_pin_interrupt(SM_COD_P);
	
	/* Initialize I2C LCD Link */
	ioport_set_pin_peripheral_mode(LCD_SDA, MUX_PA21E_TWIMS2_TWD);
	ioport_set_pin_peripheral_mode(LCD_SCK, MUX_PA22E_TWIMS2_TWCK);
	
	ioport_set_pin_peripheral_mode(PIN_PA25, MUX_PA25A_USBC_DM);
	ioport_set_pin_peripheral_mode(PIN_PA26, MUX_PA26A_USBC_DP);
		
	struct twim_config twimconf;
	twimconf.twim_clk = sysclk_get_cpu_hz();
	twimconf.speed = TWI_FAST_MODE_SPEED;
	twimconf.smbus = false;
	twimconf.hsmode_speed = 0;
	twimconf.data_setup_cycles = 0;
	twimconf.hsmode_data_setup_cycles = 0;
	
	twim_set_config(TWIM2, &twimconf);
	twim_set_callback(TWIM2, 0, twim_default_callback, 1);
	
	init_screen();
	
	/* Initialize SW0 */
	//ioport_set_pin_dir(BUTTON_0_PIN, IOPORT_DIR_INPUT);
	//ioport_set_pin_mode(BUTTON_0_PIN, IOPORT_MODE_PULLUP);
}

void user_callback_sof_action() {
	uint8_t t = 0;
	t = 3;
}

void process_report(uint8_t * report) {
	uint8_t t = 0;
	t = 3;
}

void set_feature(uint8_t * report) {
	uint8_t t = 0;
	t = 3;
}

void coder_detect() {
	// To be optimised
	//int A = digitalRead(code_A);
	//int B = digitalRead(code_B);
	
	uint16_t A = 0;
	uint16_t B = 0;
	
	uint32_t pins = ioport_get_port_level(IOPORT_PIOA, GPIO_PA04 | GPIO_PA05);
	
	A = (pins >> PIN_PA04) & 1;
	B = (pins >> PIN_PA05) & 1;

	int valid = (A << 2) | (prev_A << 3) | B | (prev_B << 1);

	prev_A = A;
	prev_B = B;

	if(valid == 0b0111 | valid == 0b1000 | valid == 0b0001 | valid == 0b1110) {
		smooth_cnt++;
		} else if (valid == 0b0100 | valid == 0b1011 | valid == 0b1101 | valid == 0b0010) {
		smooth_cnt--;
	}
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	
	init_dial();
	
	/* Init USB */
	
	udc_start();
	
	uint32_t cnt = 0;

	/* Insert application code here, after the board has been initialized. */
	while(1) {
		//sleepmgr_enter_sleep();
		cnt++;
		if(cnt == 48000000) {
			cnt = 0;
			_vfo_report.buttons++;
			udi_hid_generic_send_report_in((uint8_t *)&_vfo_report);
		}
	}
}
