/**
 * \file
 *
 * \brief Board configuration.
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

void init_dial(void);

void user_callback_sof_action(void);
void process_report(uint8_t * report);
void set_feature(uint8_t * report);

#define BOARD_OSC0_IS_XTAL true
#define BOARD_OSC0_STARTUP_US 0
#define BOARD_OSC0_HZ 16000000

#define ioport_set_pin_peripheral_mode(pin, mode) \
do {\
	ioport_set_pin_mode(pin, mode);\
	ioport_disable_pin(pin);\
} while (0)

/* Define all board peripheral */
#define RLED	PIN_PA13
#define GLED	PIN_PA31
#define BLED	PIN_PA30

#define PB1_LED	PIN_PA14
#define PB2_LED	PIN_PA15
#define PB3_LED	PIN_PA16
#define LEFT_LED	PIN_PA02
#define RIGHT_LED	PIN_PA29

#define SM_COD_N	PIN_PA05
#define SM_COD_P	PIN_PA04

#define COARSE_COD_N	PIN_PA07
#define COARSE_COD_P	PIN_PA06

#define LCD_SDA	PIN_PA21E_TWIMS2_TWD
#define LCD_SCK	PIN_PA22E_TWIMS2_TWCK

// Can also be 0x7A
#define SCR_ADDR	0x78

// IOPORT
#define IOPORT_PIOA	0

#endif /* CONF_BOARD_H_INCLUDED */
