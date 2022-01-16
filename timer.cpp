#define F_CPU 8000000UL
#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>
#include "myavrutils.h"

#define IncreasePin  B,5
#define StartPin     B,4
#define OutputPin    C,0

#define DisplayPinA    B,0
#define DisplayPinB    D,0
#define DisplayPinC    D,5
#define DisplayPinD    D,6
#define DisplayPinE    D,7
#define DisplayPinF    D,1
#define DisplayPinG    D,3
//#define DisplayPinP    B,6

#define DisplayPinCC1    D,2
#define DisplayPinCC2    D,4
#define DisplayPinCC3    B,7

bool runningState = false;
Button increaseButton;
Button startButton;

volatile int8_t digits[3] = {1, 2, 3};
volatile int8_t currDigitInd = 0;
volatile int8_t currDisplayDigitInd = 0;

volatile uint32_t timems = 0;

// Common cathode digit hex values:
uint8_t pbvalues[] = { 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01 };
uint8_t pdvalues[] = { 0xe3, 0x21, 0xc9, 0x69, 0x2b, 0x6a, 0xea, 0x21, 0xeb, 0x6b };

void DisplayDigit();
void clearDigitDisplay();
void HandleWaitingState();
void HandleRunningState();
void Setup7SegmentPins();
void StartTimer();
void setup();

void Setup7SegmentPins()
{
  SetPinAsOutput(DisplayPinA);
  SetPinAsOutput(DisplayPinB);
  SetPinAsOutput(DisplayPinC);
  SetPinAsOutput(DisplayPinD);
  SetPinAsOutput(DisplayPinE);
  SetPinAsOutput(DisplayPinF);
  SetPinAsOutput(DisplayPinG);
  //SetPinAsOutput(DisplayPinP);
	
  SetPinAsOutput(DisplayPinCC1);
  SetPinAsOutput(DisplayPinCC2);
  SetPinAsOutput(DisplayPinCC3);
}

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
  
  Setup7SegmentPins();
  StartTimer();
}

int main(void)
{
  setup();
  //SetPinStateLow(DisplayPinP);
  
  while(1)
  {
    if(!runningState)
    {
      const bool high = GetPinState(StartPin) == HIGH ? true : false;
      const ReleaseType rt = startButton.process(high);
      if(NO_PRESS != rt)
      {
        runningState = true;
        timems = (uint32_t)digits[0] + 10 * (uint32_t)digits[1] + 100 * (uint32_t)digits[2];
        timems = 1000 * timems;
      }
    }
  	
    DisplayDigit();
    currDisplayDigitInd++;
    if(currDisplayDigitInd > 2)
      currDisplayDigitInd = 0;
  	
    if(!runningState)
    {
      HandleWaitingState();
    }
    else
    {
      HandleRunningState();
      if(digits[0] == 0 && digits[1] == 0 && digits[2] == 0)
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
  {
    if(currDigitInd >= 0 && currDigitInd <= 2)
    {
      ++digits[currDigitInd];
      if(digits[currDigitInd] > 9)
        digits[currDigitInd] = 0;
    }
  }
  else if (LONG_PRESS == rt)
  {
    ++currDigitInd;
    if(currDigitInd > 2)
      currDigitInd = 0;
  }
}

void HandleRunningState()
{
  int32_t times = timems / 1000;
  digits[0] = times % 10;
	
  times = times/10;
  digits[1] = times % 10;
	
  times = times/10;
  digits[2] = times % 10;
}

ISR(TIMER2_COMP_vect)
{
  if(timems >= 20)
    timems -= 20;
  else
    timems = 0;
}

void DisplayDigit()
{
  PORTB = (PORTB & 0xFE) | pbvalues[digits[currDisplayDigitInd]];
  PORTD = (PORTD & 0x14) | pdvalues[digits[currDisplayDigitInd]];
  
  if(currDisplayDigitInd == 0)
  {
    SetPinStateLow(DisplayPinCC1);
    SetPinStateHigh(DisplayPinCC2);
    SetPinStateHigh(DisplayPinCC3);
  }
  if(currDisplayDigitInd == 1)
  {
    SetPinStateHigh(DisplayPinCC1);
    SetPinStateLow(DisplayPinCC2);
    SetPinStateHigh(DisplayPinCC3);
  }
  if(currDisplayDigitInd == 2)
  {
    SetPinStateHigh(DisplayPinCC1);
    SetPinStateHigh(DisplayPinCC2);
    SetPinStateLow(DisplayPinCC3);
  }
}

void clearDigitDisplay()
{
  PORTB &= 0xFE;
  PORTD &= 0x14;
}
