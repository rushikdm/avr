#ifndef MULTI7SEGDISPLAY_H
#define MULTI7SEGDISPLAY_H

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

class Multi7SegDisplay
{
  uint8_t  digits[2];
  uint8_t  digitMInd;
  uint8_t  digitInd;
  bool     glowDigit;
  bool     blink;
  uint16_t count;
                             //A   B   C   D   E   F  G
  const uint8_t segPins[7] = { 14, 2, 11, 12, 13, 3, 5};
  const uint8_t digitPins[2] = { 4, 6 };

  const uint8_t port_B[10] = { 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01};
  const uint8_t port_D[10] = { 0xe3, 0x21, 0xc9, 0x69, 0x2b, 0x6a, 0xea, 0x21, 0xeb, 0x6b};
    
public:  
  
  Multi7SegDisplay() : digits{5,1}, digitMInd(0), digitInd(0), glowDigit(true), blink(true), count(0) { }

  void Setup7SegmentPins()
  {
    int ind = 0;
    for(ind = 0; ind < 7; ++ind)
      SetPinAsOutput(segPins[ind]);
	
    SetPinAsOutput(digitPins[0]);
    SetPinAsOutput(digitPins[1]);
    
    for(ind = 0; ind < 7; ++ind)
      SetPinValueLow(segPins[ind]);
    
    SetPinValueHigh(digitPins[0]);
    SetPinValueHigh(digitPins[1]);
  }
 
  void ShiftDigit()
  {
    ++digitInd;
    if(digitInd > 1)
      digitInd = 0;
  }
  
  void IncrementCurrentDigit()
  {
    ++digits[digitInd];
    if(digits[digitInd] > 9)
      digits[digitInd] = 0;
  }
  
  void SetBlink(const bool iBlink)
  {
    blink = iBlink;
  }
  
  void Multiplex()
  {
    bool glow = true;
    if(blink)
    {
      if(glowDigit)
      {
        if(count >= 400)
          glowDigit = false;
      }
      else
      {
        if(count >= 800)
        {
          glowDigit = true;
          count = 0;
        }
      }
      
      if(count < 800)
        ++count;
      else
        count = 0;
      
      if( (digitInd == digitMInd) && !glowDigit )
        glow = false;
    }
    
    if(glow)
      DisplayDigit(digits[digitMInd]);
    else
    {
      const uint8_t prev_digit_pin = digitPins[digitMInd > 0 ? (digitMInd-1) : 1];
      SetPinValueHigh(prev_digit_pin);
    }

    ++digitMInd;
    if(digitMInd > 1)
      digitMInd = 0;
  }
  
  void DisplayDigit(const uint8_t digit)
  {
    if(digit > 9)
      return;

    const uint8_t prev_digit_pin = digitPins[digitMInd > 0 ? (digitMInd-1) : 1];
    SetPinValueHigh(prev_digit_pin);
    
    PORTB = (PORTB & 0b11111110) | (0b00000001 & port_B[digit]);
    PORTD = (PORTD & 0b00010100) | (0b11101011 & port_D[digit]);
    
    SetPinValueLow(digitPins[digitMInd]);
  }
  
  void ClearDisplay()
  {
    SetPinValueHigh(digitPins[0]);
    SetPinValueHigh(digitPins[1]);

    PORTB = 0x00;
    PORTD = 0x00;
  }
  
  void SetDigit(const uint8_t index, const uint8_t value)
  {
    if(index <= 1 && value <= 9)
      digits[index] = value;
  }
  
  uint8_t GetDigit(const uint8_t index)
  {
    if(index > 1)
      return 0;
      
    return digits[index];
  }
};

#endif

