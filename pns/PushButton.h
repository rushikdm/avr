#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

#include "atmega8.h"

/*
#define HIGH 1
#define LOW 0

#define _SetPinAsInput(ddr, pos) DDR##ddr &= ~(1 << DDR##ddr##pos)
#define SetPinAsInput(pin) _SetPinAsInput(pin)

#define _SetPinAsOutput(ddr, pos) DDR##ddr |= (1 << DDR##ddr##pos)
#define SetPinAsOutput(pin) _SetPinAsOutput(pin)

#define _SetPinStateLow(port, pos) PORT##port &= ~(1 << PORT##port##pos)
#define SetPinStateLow(pin) _SetPinStateLow(pin)

#define _SetPinStateHigh(port, pos) PORT##port |= (1 << PORT##port##pos)
#define SetPinStateHigh(pin) _SetPinStateHigh(pin)

#define _GetPinState(port, pos) ((PIN##port & (1 << P##port##pos)) ? HIGH : LOW)
#define GetPinState(pin) _GetPinState(pin)
*/
enum ReleaseType
{
  NO_PRESS=0,
  SIMPLE_PRESS,
  LONG_PRESS
};

class PushButton
{
  const uint8_t m_pin;
  uint16_t m_press_count;
  uint16_t m_release_count;
  ReleaseType m_lastEvent;
	
  public:
  Button(const uint8_t pin) : m_pin{pin}, m_press_count{0}, m_release_count{0}, m_lastEvent{NO_PRESS} { }
	
  ReleaseType process()
  {
    if(IsPinHigh(m_pin))
    {
      if(m_press_count < 10000)
        ++m_press_count;
	    
      m_release_count = 0;
    }
    else
    {
      if(m_release_count < 10000)
        ++m_release_count;
	    
      m_press_count = 0;
    }

    if(m_press_count > 60)
      m_lastEvent = SIMPLE_PRESS;

    if(m_press_count > 800)
      m_lastEvent = LONG_PRESS;

    if(m_release_count > 60)
    {
      ReleaseType temp = m_lastEvent;
      m_lastEvent = NO_PRESS;
      return temp;
    }

    return NO_PRESS;
  }
};

#endif
