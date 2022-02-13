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

#define SegmentA      C,5
#define SegmentB      C,4
#define SegmentC      C,3
#define SegmentD      C,1
#define SegmentE      C,0
#define SegmentF      D,0
#define SegmentG      D,1

#define Digit1Pin B,5
#define Digit2Pin B,4


class Multi7SegDisplay
{
  const uint8_t segData[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
  uint8_t digits[3] = {3, 2, 1};
  uint8_t digitMInd = 0;
  uint8_t segMInd = 0;
  uint8_t digitInd = 0;
                             //A   B   C   D   E   F  G
  const uint8_t segPins[7] = { 14, 2, 11, 12, 13, 3, 5};
  const uint8_t digitPins[3] = { 4, 6, 10};
  
  bool present(const uint8_t idigit, const uint8_t iseg)
  {
    if(idigit < 10 && iseg < 8)
      if(segData[idigit] & (1 << iseg))
        return true;
    return false;
  }
  
public:  
  
  Multi7SegDisplay() 
  {
    digits[0] = 1;
    digits[1] = 2;
  }

  void Setup7SegmentPins()
  {
    int ind = 0;
    for(ind = 0; ind < 7; ++ind)
      SetPinAsOutput(segPins[ind]);
	
    SetPinAsOutput(digitPins[0]);
    SetPinAsOutput(digitPins[1]);
    SetPinAsOutput(digitPins[2]);
    
    for(ind = 0; ind < 7; ++ind)
      SetPinValueHigh(segPins[ind]);
    
    SetPinValueLow(digitPins[0]);
    SetPinValueLow(digitPins[1]);
    SetPinValueLow(digitPins[2]);
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
    if(digits[digitInd] > 9)
      digits[digitInd] = 0;
  }
  
  void Multiplex()
  {
    const uint8_t prev_pin = segPins[segMInd > 0 ? (segMInd-1) : 6];
    SetPinValueHigh(prev_pin);
    
    if(segMInd == 0)
    {
      const uint8_t prev_digit = digitPins[digitMInd > 0 ? (digitMInd-1) : 1];
      SetPinValueLow(prev_digit);
      
      SetPinValueHigh(digitPins[digitMInd]);
    }
    
    if(present(digits[digitMInd], segMInd))
      SetPinValueLow(segPins[segMInd]);
    
    ++segMInd;
    if(segMInd > 6)
    {
      segMInd = 0;
      ++digitMInd;
      if(digitMInd > 2)
        digitMInd = 0;
    }
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
};

#endif
