#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

#include "atmega8.h"

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
  PushButton(const uint8_t pin) : m_pin{pin}, m_press_count{0}, m_release_count{0}, m_lastEvent{NO_PRESS} { }

  setup()
  {
    SetPinAsInput(m_pin);
  }
	
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
