#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "myavrutils.h"
#include "multi7segment.h"

#define IncreasePin  B,5
#define StartPin     B,4
#define OutputPin    C,0

bool runningState = false;
Button increaseButton;
Button startButton;

Multiplexed7Segment display;

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
  SetPinAsOutput(OutputPin);
  
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
      const bool high = GetPinState(StartPin) == HIGH ? true : false;
      const ReleaseType rt = startButton.process(high);
      if(NO_PRESS != rt)
      {
        runningState = true;
        timems = (uint32_t)display.GetDigit(0) + 10 * (uint32_t)display.GetDigit(1) + 100 * (uint32_t)display.GetDigit(2);
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
        SetPinStateLow(OutputPin);
        break;
      }
    }
  }

  return 0;
}

void HandleWaitingState()
{  		
  const bool high = GetPinState(IncreasePin) == HIGH ? true : false;
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
	
  times = times/10;
  digit = times % 10;
  display.SetDigit(2, digit);
}

ISR(TIMER2_COMP_vect)
{
  if(timems >= 20)
    timems -= 20;
  else
    timems = 0;
}

