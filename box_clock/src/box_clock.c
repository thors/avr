/* box_clock.c
 * The following program just  keeps toggling pin 0 of port B
 */

#include <avr/io.h> //standard include for ATMega16
#include <avr/interrupt.h> //standard include for ATMega16
//#include "avr_mcu_section.h"
//AVR_MCU(F_CPU, "attiny85");

#define sbi(x,y) x |= _BV(y) //set bit - using bitwise OR operator
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator
#define is_high(x,y) (x & _BV(y) == _BV(y)) //check if the y'th bit of register 'x' is high ... test if its AND with 1 is 1

/* _BV(a) is a macro which returns the value corresponding to 2 to the power 'a'. Thus _BV(PX3) would be 0x08 or 0b00001000 */

int seconds;
int state;
int counter;
int counter2;
int beep;


//Starts with idle, after 50 seconds indicate imminent start
#define PRESTART  50
// +10s: Round starts
#define STARTED  60
// +110s: Warning, round will end
#define NEAREND 170
// +10s: round ends
#define ENDED 180


#define GREEN_ON sbi(PORTB,PB0)
#define GREEN_OFF cbi(PORTB,PB0)
#define ORANGE_ON sbi(PORTB,PB1)
#define ORANGE_OFF cbi(PORTB,PB1)
#define RED_ON sbi(PORTB,PB2)
#define RED_OFF cbi(PORTB,PB2)
#define BEEP_ON sbi(PORTB,PB3)
#define BEEP_OFF cbi(PORTB,PB3)

void init_timer();

int main(void) {
  seconds = PRESTART;
  state = PRESTART;
  counter2 = 0;
  beep = 0;
  DDRB=0xff; //PORTB as OUTPUT
  PORTB=0x00;
  cli();
  init_timer();
  // Global Interrupts aktivieren
  sei();
  GREEN_OFF;
  ORANGE_ON;
  RED_ON;
  while(1) {
    if (beep > 0) {
       BEEP_ON;
    } else {
       BEEP_OFF; 
    }
    switch(state) {
    case PRESTART:
      // Set red+orange
      if (seconds >= STARTED) {
	state = STARTED;
        beep = 5;
	GREEN_ON;
	ORANGE_OFF;
	RED_OFF;
      }
      break;
    case STARTED:
      // Set green
      if (seconds >= NEAREND) {
	state = NEAREND;
        beep = 1;
	GREEN_OFF;
	ORANGE_ON;
	RED_OFF;
      }
      break;
    case NEAREND:
      // Set orange
      if (seconds >= ENDED) {
	state = ENDED;
	GREEN_OFF;
	ORANGE_OFF;
	RED_ON;
        beep = 5;
	seconds = 0;
      }
      break;
    case ENDED:
      // Set red
      if (seconds >= PRESTART) {
	state = PRESTART;
	GREEN_OFF;
	ORANGE_ON;
	RED_ON;
      }
      break;
    }
  }
  return 0;
}

//ISR (TIMER0_COMPA_vect) {
ISR (TIM0_COMPA_vect) { 
  if (counter++ > 125)  {
    counter = 0;
    seconds++;
    if (beep > 0) {
      beep--;
    }
  }
}

ISR(BADISR_vect)
{
  RED_ON;
  // user code here
}

void init_timer() {
  // CPU freq = 8MHz = 64*125*125
  // set pre_scaler 64
  // set counter to 125
  // Timer 0 konfigurieren
  TCCR0A = (1<<WGM01); // CTC Modus
  TCCR0B = (1<<CS01) | (1<<CS00); // Prescaler 64
  OCR0A = 124; //
  // Compare Interrupt erlauben
  TIMSK |= (1<<OCIE0A);
}
