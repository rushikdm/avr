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
/*
#define SegmentA      B,0
#define SegmentB      D,0
#define SegmentC      D,5
#define SegmentD      D,6
#define SegmentE      D,7
#define SegmentF      D,1
#define SegmentG      D,3

#define Digit1Pin B,5
#define Digit2Pin B,4
#define Digit3Pin B,4

*/

class Multi7SegDisplay
{
  uint8_t  digits[3];
  uint8_t  digitMInd;
  uint8_t  digitInd;
  bool     glowDigit;
  bool     blink;
  uint16_t count;
                             //A   B   C   D   E   F  G
  const uint8_t segPins[7] = { 14, 2, 11, 12, 13, 3, 5};
  const uint8_t digitPins[3] = { 4, 6, 10};

  const uint8_t port_B[10] = { 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01};
  const uint8_t port_D[10] = { 0xe3, 0x21, 0xc9, 0x69, 0x2b, 0x6a, 0xea, 0x21, 0xeb, 0x6b};
    
public:  
  // Default time = 1 hour 20 minutes.
  Multi7SegDisplay() : digits{0,2,1}, digitMInd(0), digitInd(0), glowDigit(true), blink(true), count(0) { }

  void Setup7SegmentPins()
  {
    int ind = 0;
    for(ind = 0; ind < 7; ++ind)
      SetPinAsOutput(segPins[ind]);
	
    SetPinAsOutput(digitPins[0]);
    SetPinAsOutput(digitPins[1]);
    SetPinAsOutput(digitPins[2]);
    
    for(ind = 0; ind < 7; ++ind)
      SetPinValueLow(segPins[ind]);
    
    SetPinValueHigh(digitPins[0]);
    SetPinValueHigh(digitPins[1]);
    SetPinValueHigh(digitPins[2]);
  }
 
  void ShiftDigit()
  {
    ++digitInd;
    if(digitInd > 2)
      digitInd = 0;
  }
  
  void IncrementCurrentDigit()
  {
    ++digits[digitInd];
	if(1 == digitInd)
	{
	  // Middle digit with index = 1 is for the minutes display and its value cannot exceed 5.
	  // It is not possible to have more than 59 value for minutes.
	  if(5 < digits[digitInd])
	    digits[digitInd] = 0;
	}
    else 
	{
	  if(digits[digitInd] > 9)
        digits[digitInd] = 0;
	}
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
      const uint8_t prev_digit_pin = digitPins[digitMInd > 0 ? (digitMInd-1) : 2];
      SetPinValueHigh(prev_digit_pin);
    }

    ++digitMInd;
    if(digitMInd > 2)
      digitMInd = 0;
  }
  
  void DisplayDigit(const uint8_t digit)
  {
    if(digit > 9)
      return;

    const uint8_t prev_digit_pin = digitPins[digitMInd > 0 ? (digitMInd-1) : 2];
    SetPinValueHigh(prev_digit_pin);
    
    PORTB = (PORTB & 0b11111110) | (0b00000001 & port_B[digit]);
    PORTD = (PORTD & 0b00010100) | (0b11101011 & port_D[digit]);
    
    SetPinValueLow(digitPins[digitMInd]);
  }
  
  void ClearDisplay()
  {
    SetPinValueHigh(digitPins[0]);
    SetPinValueHigh(digitPins[1]);
    SetPinValueHigh(digitPins[2]);
    PORTB = 0x00;
    PORTD = 0x00;
  }
  
  void SetDigit(const uint8_t index, const uint8_t value)
  {
    if(index <= 2 && value <= 9)
      digits[index] = value;
  }
  
  uint8_t GetDigit(const uint8_t index)
  {
    if(index > 2)
      return 0;
      
    return digits[index];
  }
  
  bool IsZero()
  {
    return (0 == digits[0] && 0 == digits[1] && 0 == digits[2]);
  }
  
  void Decrease()
  {
    if(0 < digits[0])
		--digits[0];
	else
	{
		if(0 < digits[1])
		{
			--digits[1];
			digits[0] = 9;
		}
		else
		{
			if(0 < digits[2])
			{
				digits[0] = 9;
				digits[1] = 5;
				--digits[2];
			}
		}
	}
  }
};

#endif