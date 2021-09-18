#include <HID-Project.h>
#include <HID-Settings.h>
#include "vfo.h"

int mode_select = 10;
int code_A = 2;
int code_B = 3;

void setup() {
  pinMode(mode_select, INPUT_PULLUP);
  pinMode(code_A, INPUT_PULLUP);
  pinMode(code_B, INPUT_PULLUP);

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 625;              // compare match register 16MHz/256/625 => 10ms
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts


  attachInterrupt(digitalPinToInterrupt(code_A), coder_detect, CHANGE);
  attachInterrupt(digitalPinToInterrupt(code_B), coder_detect, CHANGE);

  //SurfaceDial.begin();
  VFODial.begin();
}

volatile int prev_A = 0;
volatile int prev_B = 0;
volatile int counter = 0;
volatile int freeze_cnt = 0;
volatile int send_report = 0;

volatile uint8_t button_state = 0;

void coder_detect() {
  // To be optimised
  int A = digitalRead(code_A);
  int B = digitalRead(code_B);

  int valid = (A << 2) | (prev_A << 3) | B | (prev_B << 1);

  prev_A = A;
  prev_B = B;

  if(valid == 0b0111 | valid == 0b1000 | valid == 0b0001 | valid == 0b1110) {
    counter++;
  } else if (valid == 0b0100 | valid == 0b1011 | valid == 0b1101 | valid == 0b0010) {
    counter--;
  }
}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  if(counter != 0) {
    freeze_cnt = counter;
    counter = 0;
    send_report = 1;
  }
  uint8_t bstat = digitalRead(mode_select);
  if(bstat != button_state) {
    button_state = bstat;
    send_report = 1;
  }
}

void loop() {
  if(send_report) {
    //SurfaceDial.rotate(freeze_cnt);
    send_report = 0;
    VFODial.sendstatus(button_state, freeze_cnt, 0);
    freeze_cnt = 0;
    /*send_report = 0;*/
  }
}
