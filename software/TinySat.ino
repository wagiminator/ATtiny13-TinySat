// ===================================================================================
// Project:   TinySat - Solar Powered Blinken Lights based on ATtiny13A
// Version:   v1.0
// Year:      2021
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Wiring:
// -------
//                         +-\/-+
//      --- RST ADC0 PB5  1|°   |8  Vcc
//      ------- ADC3 PB3  2|    |7  PB2 ADC1 -------- 
//      ------- ADC2 PB4  3|    |6  PB1 AIN1 OC0B --- LED1 PWM
//                   GND  4|    |5  PB0 AIN0 OC0A --- LED0 PWM
//                         +----+
//
// Compilation Settings:
// ---------------------
// Controller:  ATtiny13A
// Core:        MicroCore (https://github.com/MCUdude/MicroCore)
// Clockspeed:  128 kHz internal
// BOD:         BOD disabled
// Timing:      Micros disabled
//
// Leave the rest on default settings. Don't forget to "Burn bootloader"!
// No Arduino core functions or libraries are used. Use the makefile if 
// you want to compile without Arduino IDE.
//
// Fuse settings: -U lfuse:w:0x3b:m -U hfuse:w:0xff:m


// ===================================================================================
// Libraries and Definitions
// ===================================================================================

// Libraries
#include <avr/io.h>             // for gpio
#include <avr/sleep.h>          // for the sleep modes
#include <avr/interrupt.h>      // for the interrupts
#include <avr/wdt.h>            // for the watchdog timer

// Pin definitions
#define LED0          PB0       // pin for LED 0
#define LED1          PB1       // pin for LED 1

// ===================================================================================
// Watchdog Timer Implementation
// ===================================================================================

// Reset watchdog timer
void resetWatchdog(void) {
  cli();                                  // timed sequence coming up
  wdt_reset();                            // reset watchdog
  MCUSR = 0;                              // clear various "reset" flags
  WDTCR = (1<<WDCE)|(1<<WDE)|(1<<WDTIF);  // allow changes, clear interrupt
  WDTCR = (1<<WDTIE)|(1<<WDP3);           // set WDT interval 4 seconds
  sei();                                  // interrupts are required now
}

// Watchdog interrupt service routine
ISR (WDT_vect) {
  wdt_disable();                          // disable watchdog
}

// ===================================================================================
// Main Function
// ===================================================================================

// Main function
int main(void) {
  // Reset watchdog timer
  resetWatchdog();                        // do this first in case WDT fires
  
  // PWM setup
  TCCR0A = (1<<COM0A1) | (1<<COM0B1)      // clear OC0A/OC0B on compare match, set at TOP
         | (1<<WGM01)  | (1<<WGM00);      // fast PWM 0x00 - 0xff
  TCCR0B = (1<<CS00);                     // start timer without prescaler
  TIMSK0 = (1<<TOIE0);                    // timer/counter0 overflow interrupt enable

  // Setup pins
  DDRB   = (1<<LED0) | (1<<LED1);         // LED pins as output
  
  // Disable unused peripherals to save power
  ADCSRA = 0;                             // disable ADC
  ACSR   = (1<<ACD);                      // disable analog comperator
  PRR    = (1<<PRADC);                    // shut down ADC
  DIDR0  = 0x1F;                          // disable digital intput buffer
  
  // Loop
  while(1) {
    // Fade in/out LED 0
    uint8_t i = 0;
    set_sleep_mode(SLEEP_MODE_IDLE);
    while(++i) {
      OCR0A = i;
      sleep_mode();
    }
    while(--i) {
      OCR0A = i;
      sleep_mode();
    }
    
    // Sleep for 4 seconds
    OCR0A = 0;
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    resetWatchdog();
    sleep_mode();
    
    // Fade in/out LED 1
    set_sleep_mode(SLEEP_MODE_IDLE);
    while(++i) {
      OCR0B = i;
      sleep_mode();
    }
    while(--i) {
      OCR0B = i;
      sleep_mode();
    }
    
    // Sleep for 4 seconds
    OCR0B = 0;
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    resetWatchdog();
    sleep_mode(); 
  }
}

// Timer0 overflow interrupt service routine
EMPTY_INTERRUPT(TIM0_OVF_vect);     // nothing to be done here, just wake up from sleep
