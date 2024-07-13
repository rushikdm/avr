#define F_CPU 8000000UL
#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>

#include "atmega8.h"
#include "PNSRange.h"
#include "PNSDisplay.h"
#include "PushButton.h"

/*
avr-gcc -Wall -Os -mmcu=atmega8 -o pns.o pns.cpp
avr-objcopy -j .text -j .data -O ihex pns.o pns.hex
avrdude -p ATmega8 -c arduino -P COM3 -b 19200 -U flash:w:pns.hex:i
*/

#define LOW 0
#define HIGH 1

#define PWMPin              15
#define TwentyMSPin          3
#define BuzzerPin            2
#define CircuitCompletePin  14

#define IncreasePWMPin       5
#define DecreasePWMPin       4

PushButton increasePWMButton(IncreasePWMPin);
PushButton decreasePWMButton(DecreasePWMPin);

PNSRange pnsRange;
PNSDisplay pnsDisplay(pnsRange.getAmpereValue());

volatile uint8_t pwm2_ind = 0;

void setup();
void setupPWM();
void setupTimer();
void increasePWM();
void decreasePWM();

void setup(void)
{ 
  SetPinAsOutput(PWMPin);
  SetPinAsOutput(TwentyMSPin);
  SetPinAsOutput(BuzzerPin);
  SetPinAsInput(CircuitCompletePin);
    
  increasePWMButton.setup();
  decreasePWMButton.setup();
  pnsDisplay.setup();

  setupPWM();
  setupTimer();
    
  sei(); // Enable Global Interrupt
}

void setupPWM()
{
  // set non-inverting mode
  TCCR1A |= (1 << COM1A1);
        
  // set 10bit phase corrected PWM Mode
  TCCR1A |= (1 << WGM11) | (1 << WGM10);
        
  // set prescaler to 8 and starts PWM
  TCCR1B |= (1 << CS11);
        
  // set PWM duty cycle
  OCR1A = pnsRange.getPWMValue();
}

void setupTimer()
{
  /* Start of initialization for timer in CTC mode */

  OCR2 = 16;
    
  TCCR2 |= (1 << WGM21);
  // Set to CTC Mode

  TIMSK |= (1 << OCIE2);
  //Set interrupt on compare match

  TCCR2 |= (1 << CS20) | (1 << CS21) | (1 << CS22);
  // set prescaler to 1024
    
  /* End of initialization for timer in CTC mode */
}

int main(void)
{
  setup();   
  while (1)
  {
    if(SIMPLE_PRESS == increasePWMButton.process())
      increasePWM();
    else if(SIMPLE_PRESS == decreasePWMButton.process())
      decreasePWM();

    pnsDisplay.multiplex();
  }  
}

void increasePWM()
{
  pnsRange.increase();
  OCR1A = pnsRange.getPWMValue();
  pnsDisplay.setAmpereValue(pnsRange.getAmpereValue());
}

void decreasePWM()
{
  pnsRange.decrease();
  OCR1A = pnsRange.getPWMValue();
  pnsDisplay.setAmpereValue(pnsRange.getAmpereValue());
}

ISR(TIMER2_COMP_vect)
{
  if(IsPinHigh(CircuitCompletePin))
  {
    if(pwm2_ind <= 0)
      SetPinValueHigh(TwentyMSPin);
    else
      SetPinValueLow(TwentyMSPin);
    
    if(pwm2_ind <= 8)
      SetPinValueHigh(BuzzerPin);
    else
      SetPinValueLow(BuzzerPin);
  
    pwm2_ind++;
    if(pwm2_ind >= 50)
      pwm2_ind = 0;
  }
  else
  {
    SetPinValueLow(TwentyMSPin);
    SetPinValueLow(BuzzerPin);
  }    
}
