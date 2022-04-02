#define F_CPU 1000000UL
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

#define IncreasePin  24
#define StartPin     25
#define BuzzerPin    19

volatile bool runningState = false;
volatile bool time_changed = false;
volatile uint32_t time_ms = 0;

Button increaseButton;
Button startButton;

Multi7SegDisplay display;

void HandleWaitingState();
void HandleRunningState();
void StartTimer();
void setup();
void beep();

void StartTimer()
{
  /* Start of initialization for timer in CTC mode */
    
  OCR2 = 244;
    
  TCCR2 |= (1 << WGM21);
  // Set to CTC Mode

  TIMSK |= (1 << OCIE2);
  // Set interrupt on compare match

  TCCR2 |= (1 << CS21);
  // set prescaler to 8
    
  /* End of initialization for timer in CTC mode */
    
  sei(); //Enable Global Interrupt
}

void setup()
{
  SetPinAsInput(IncreasePin);
  SetPinAsInput(StartPin);
  SetPinAsOutput(BuzzerPin);
  
  display.Setup7SegmentPins();
  StartTimer();
}

int main(void)
{
  _delay_ms(100);
  setup();
  
  _delay_ms(100);
  display.SetBlink(true);

  while(!runningState)
  {
    display.Multiplex();
    HandleWaitingState();
  }
  
  display.SetBlink(false);
  
  while(!display.IsZero())
  {
    display.Multiplex();
    HandleRunningState();
  }

  beep();
  display.ClearDisplay();

  return 0;
}

void HandleRunningState()
{
  if(time_changed)
  {
    time_changed = false;
    if(time_ms < 30000)
      ++time_ms;
    else
    {
      time_ms = 0;
      display.Decrease();
    }
  }
}

void HandleWaitingState()
{
  bool high = IsPinHigh(StartPin) ? true : false;
  ReleaseType rt = startButton.process(high);
  if(NO_PRESS != rt)
  {
    runningState = true;
    return;
  }
    
  high = IsPinHigh(IncreasePin) ? true : false;
  rt = increaseButton.process(high);
 		
  if(SIMPLE_PRESS == rt)
    display.IncrementCurrentDigit();
  else if (LONG_PRESS == rt)
    display.ShiftDigit();
}

ISR(TIMER2_COMP_vect)
{
  time_changed = true;
}

void beep()
{
  uint8_t ind = 0;
  for(ind = 0; ind < 20; ++ind)
  {
    display.Multiplex();
    SetPinValueHigh(BuzzerPin);
    _delay_ms(200);
    SetPinValueLow(BuzzerPin);
    _delay_ms(1800);
  }
}

