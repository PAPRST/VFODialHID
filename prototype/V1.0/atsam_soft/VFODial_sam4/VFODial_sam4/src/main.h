/*
 * main.h
 *
 * Created: 07/11/2021 18:25:56
 *  Author: nats
 */ 


#ifndef MAIN_H_
#define MAIN_H_

void coder_detect(void);

// Globals
int16_t smooth_cnt = 0;
int8_t prev_A = 0;
int8_t prev_B = 0;

#define TC0_CHAN0	0
#define TC_CAPTURE_MODE	0
#define TC_WAVE_MODE	0x8000

#endif /* MAIN_H_ */