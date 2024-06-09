#define F_CPU 8000000UL
#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>
//#include "myavrutils.h"
#include "atmega8.h"
#include "PNSRange.h"
#include "PNSDisplay.h"

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

#define IncreasePWMPin       4
#define DecreasePWMPin       5

void setup();
void setupPWM();
void setupInterrupts();
void setupTimer();

PNSRange pnsRange;
PNSDisplay pnsDisplay(pnsRange.getAmpereValue());

volatile uint8_t pwm2_ind = 0;

void setup(void)
{ 
    SetPinAsOutput(PWMPin);
    SetPinAsOutput(TwentyMSPin);
    SetPinAsOutput(BuzzerPin);
    SetPinAsInput(CircuitCompletePin);
    
    SetPinAsInput(IncreasePWMPin);
    SetPinAsInput(DecreasePWMPin);
    
    pnsDisplay.setup();
    setupPWM();
    setupInterrupts();
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

void setupInterrupts()
{    
    /* Start of Interrupt initialization for pin PD2 (4), PD3(5) */

      GICR = (1 << INT0) | (1 << INT1);// Enable INT0 and INT1
	    MCUCR = (1 << ISC01) | (1 << ISC00) | (1 << ISC11) | (1 << ISC10) ;	// Trigger INT0 & INT1 on rising edge
	    
    /* End of Interrupt initialization for pin PD2 (4), PD3(5) */
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
      pnsDisplay.multiplex();
    }  
}

ISR(INT0_vect)
{
    pnsRange.increase();
    OCR1A = pnsRange.getPWMValue();
    pnsDisplay.setAmpereValue(pnsRange.getAmpereValue());
}

ISR(INT1_vect)
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
