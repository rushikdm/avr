#ifndef PNS_DISPLAY_H
#define PNS_DISPLAY_H

#include "atmega8.h"

/*
     A
   -------          
   |     | 
 F |     | B
   |-----|
   |  G  |
 E |     | C
   |     |
   -------
      D
      
          G   F   E   D   C   B   A
      0   0   1   1   1   1   1   1   0x3F
      1   0   0   0   0   1   1   0   0x06
      2   1   0   1   1   0   1   1   0x5B
      3   1   0   0   1   1   1   1   0x4F
      4   1   1   0   0   1   1   0   0x66
      5   1   1   0   1   1   0   1   0x6D
      6   1   1   1   1   1   0   1   0x7D
      7   0   0   0   0   1   1   1   0x07
      8   1   1   1   1   1   1   1   0x7F
      9   1   1   0   1   1   1   1   0x6F

*/

class PNSDisplay
{
	uint8_t  m_ampere_value;
  uint8_t  m_digits[2];
  uint8_t  m_multiplex_index;
  uint8_t  digitInd;
  bool     glowDigit;
  bool     blink;
  uint16_t count;
                                 //A   B   C   D   E   F  G
  const uint8_t m_segmentPins[7] = { 14, 2,  11, 12, 13, 3, 5};
  const uint8_t m_fractionPin = 10; // To-do change P pin
  const uint8_t m_digitCCPins[2] = { 4, 6 }; // Common Cathode pins for two segments
   
public:  
  
  PNSDisplay() : m_digits{2,1}, m_multiplex_index(0), digitInd(0), glowDigit(true), blink(true), count(0) { }

  void setup()
  {
    int ind = 0;
    for(ind = 0; ind < 7; ++ind)
      SetPinAsOutput(m_segmentPins[ind]);
	
		SetPinAsOutput(m_fractionPin);
    SetPinAsOutput(m_digitCCPins[0]);
    SetPinAsOutput(m_digitCCPins[1]);
    
    clearDisplay();
  }
 
  void clearDisplay()
  {
  	int ind;
    for(ind = 0; ind < 7; ++ind)
      SetPinValueLow(m_segmentPins[ind]);

		SetPinValueLow(m_fractionPin);
	  SetPinValueHigh(m_digitCCPins[0]);
    SetPinValueHigh(m_digitCCPins[1]);
  }
  
  void setBlink(const bool iBlink)
  {
    blink = iBlink;
  }
  
  void multiplex()
  {
    bool glow = true;
    if(blink)
    {
      if(glowDigit)
      {
        if(count >= 600)
          glowDigit = false;
      }
      else
      {
        if(count >= 1200)
        {
          glowDigit = true;
          count = 0;
        }
      }
      
      if(count < 1200)
        ++count;
      else
        count = 0;
      
      if( (digitInd == m_multiplex_index) && !glowDigit )
        glow = false;
    }
    
    if(glow)
      displayDigit(m_digits[m_multiplex_index]);
    else
    {
      const uint8_t prev_digit_pin = m_digitCCPins[m_multiplex_index > 0 ? (m_multiplex_index-1) : 1];
      SetPinValueHigh(prev_digit_pin);
    }

    ++m_multiplex_index;
    if(m_multiplex_index > 1)
      m_multiplex_index = 0;
  }
  
  void displayDigit(const uint8_t digit)
  {
    if(digit > 9)
      return;

    const uint8_t prev_digit_pin = m_digitCCPins[m_multiplex_index > 0 ? (m_multiplex_index-1) : 1];
    SetPinValueHigh(prev_digit_pin);
    
    //PORTB = (PORTB & 0b11111110) | (0b00000001 & port_B[digit]);
    //PORTD = (PORTD & 0b00010100) | (0b11101011 & port_D[digit]);
    
    SetPinValueLow(m_digitCCPins[m_multiplex_index]);
  }
  
  void setAmpereValue(const uint8_t value)
  {
    if(value <= 5)
      m_ampere_value = value;
  }
  
};

#endif
