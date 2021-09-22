#include <splash.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include <HID-Project.h>
#include <HID-Settings.h>
#include "vfo.h"

#define OLED_RESET 4
Adafruit_SSD1306 display_oled(OLED_RESET);

int mode_select = 10;
int code_A = 0;
int code_B = 1;
int led = 14;

void setup() {
  pinMode(mode_select, INPUT_PULLUP);
  pinMode(code_A, INPUT_PULLUP);
  pinMode(code_B, INPUT_PULLUP);
  pinMode(led, OUTPUT);

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

  VFO.begin();
  //VFO.setFeatureReport(text, sizeof(text));
  VFO.enableFeatureReport();

  display_oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_oled.clearDisplay();

  display_oled.setTextSize(1);
  display_oled.setTextColor(1);
  display_oled.setCursor(0,0);
  display_oled.print("PAPR VFO KNOB");
  display_oled.display();

  digitalWrite(led,  0);
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
    VFO.setSmooth(counter);
    counter = 0;
    send_report = 1;
  }
  uint8_t bstat = digitalRead(mode_select);
  if(bstat != button_state) {
    button_state = bstat;
    VFO.setButtons(bstat);
    send_report = 1;
  }
}

uint8_t led_status = 0;
uint8_t btext[64] = {0};
uint8_t bleds[3] = {0};
uint8_t display_update = 0;
uint8_t leds_update = 0;

void loop() {
  if(send_report) {
    send_report = 0;
    VFO.sendReport();
    VFO.setSmooth(0);
    VFO.setButtons(0);
  }
  if(VFO.isDataRdy()) {
    uint8_t * text = VFO.getTxt();
    
    uint8_t cmd = text[0];

    if(cmd == DISPLAY_CMD) {
      //strncpy((char *)btext, (char*) text, 64);
      for(uint8_t i = 0; i < 64; i++) {
        btext[i] = text[i];
        if (i > 2 && text[i] == 0) {
          break;
        }
      }
      display_update = 1;
    } else if(cmd == LED_CMD) {
      for(uint8_t i = 0; i < 3; i++) {
        bleds[i] = text[i];
      }
      leds_update = 1;
    }
    VFO.rstDataRdy();
    VFO.enableFeatureReport();
  }

  if(leds_update) {
    uint16_t led_status = bleds[1] << 8 | bleds[2];
    if(led_status) {
      digitalWrite(led, 1);
    } else {
      digitalWrite(led, 0);
    }

    leds_update = 0;
  }
  if(display_update) {
    uint8_t x = btext[1];
    uint8_t y = btext[2];
    display_oled.setCursor(x,y);
    display_oled.clearDisplay();
    
    uint8_t * rtxt = &btext[3];
    display_oled.print((char *)rtxt);
    display_oled.display();

    display_update = 0;
  }
}
