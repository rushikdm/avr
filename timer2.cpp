#define F_CPU 8000000UL
//#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "myavrutils.h"
#include "Multi7SegDisplay.h"

/*
avr-gcc -Wall  -Os -mmcu=atmega8a -o timer2.o timer2.cpp -std=c++11
avr-objcopy -j .text -j .data -O ihex timer2.o timer2.hex
avrdude -p ATmega8 -c arduino -P COM18 -b 19200 -U flash:w:timer2.hex:i -F
*/

//#define IncreasePin  D,2
//#define StartPin     D,3
//#define OutputPin    C,0
#define IncreasePin  4
#define StartPin     5

bool runningState = false;
Button increaseButton;
Button startButton;

Multi7SegDisplay display;

volatile uint32_t timems = 0;

void HandleWaitingState();
void HandleRunningState();
void StartTimer();
void setup();

void StartTimer()
{
  /* Start of initialization for timer in CTC mode */
    
  OCR2 = 16;
    
  TCCR2 |= (1 << WGM21);
  // Set to CTC Mode

  TIMSK |= (1 << OCIE2);
  // Set interrupt on compare match

  TCCR2 |= (1 << CS20) | (1 << CS21) | (1 << CS22);
  // set prescaler to 1024
    
  /* End of initialization for timer in CTC mode */
    
  sei(); //Enable Global Interrupt
}

void setup()
{
  SetPinAsInput(IncreasePin);
  SetPinAsInput(StartPin);
  //SetPinAsOutput(OutputPin);
  
  display.Setup7SegmentPins();
  StartTimer();
}

int main(void)
{
  _delay_ms(500);
  setup();
  
  _delay_ms(500);
  
  while(1)
  {
    if(!runningState)
    {
      const bool high = IsPinHigh(StartPin) ? true : false;
      const ReleaseType rt = startButton.process(high);
      if(NO_PRESS != rt)
      {
        runningState = true;
        timems = (uint32_t)display.GetDigit(0) + 10 * (uint32_t)display.GetDigit(1);
        timems = 1000 * timems;
      }
    }
  	
    display.Multiplex();
  	
    if(!runningState)
    {
      HandleWaitingState();
    }
    else
    {
      HandleRunningState();
      if(timems == 0)
      {
        //SetPinStateLow(OutputPin);
        break;
      }
    }
  }

  return 0;
}

void HandleWaitingState()
{  		
  const bool high = IsPinHigh(IncreasePin) ? true : false;
  const ReleaseType rt = increaseButton.process(high);
 		
  if(SIMPLE_PRESS == rt)
    display.IncrementCurrentDigit();
  else if (LONG_PRESS == rt)
    display.ShiftDigit();
}

void HandleRunningState()
{
  int32_t times = timems / 1000;
  uint8_t digit = times % 10;
  display.SetDigit(0, digit);
	
  times = times/10;
  digit = times % 10;
  display.SetDigit(1, digit);
}

ISR(TIMER2_COMP_vect)
{
  if(timems >= 20)
    timems -= 20;
  else
    timems = 0;
}


