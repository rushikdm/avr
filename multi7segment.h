#ifndef MULTIPLEXED_7SEGMENT
#define MULTIPLEXED_7SEGMENT

#include "myavrutils.h"

#define SegmentA    B,0
#define SegmentB    D,0
#define SegmentC    D,5
#define SegmentD    D,6
#define SegmentE    D,7
#define SegmentF    D,1
#define SegmentG    D,3
#define SegmentP    B,6

#define Digit1Pin D,2
#define Digit2Pin D,4
#define Digit3Pin B,7

class Multiplexed7Segment
{
  uint8_t digits[3];
  uint8_t currInd;
  uint8_t digitInd;
  
  void DisplayDigit()
  {
    if(digits[currInd] == 1 || digits[currInd] == 4)
      SetPinStateLow(SegmentA);
    else
      SetPinStateHigh(SegmentA);
    
    if(digits[currInd] == 5 || digits[currInd] == 6)
      SetPinStateLow(SegmentB);
    else
      SetPinStateHigh(SegmentB);
      
    if(digits[currInd] == 2)
      SetPinStateLow(SegmentC);
    else
      SetPinStateHigh(SegmentC);
      
    if(digits[currInd] == 1 || digits[currInd] == 4 || digits[currInd] == 7)
      SetPinStateLow(SegmentD);
    else
      SetPinStateHigh(SegmentD);

    if(digits[currInd] == 0 || digits[currInd] == 2 || digits[currInd] == 6  || digits[currInd] == 8)
      SetPinStateHigh(SegmentE);
    else
      SetPinStateLow(SegmentE);

    if(digits[currInd] == 1 || digits[currInd] == 2 || digits[currInd] == 3  || digits[currInd] == 7)
      SetPinStateLow(SegmentF);
    else
      SetPinStateHigh(SegmentF);
      
    if(digits[currInd] == 0 || digits[currInd] == 1 || digits[currInd] == 7)
      SetPinStateLow(SegmentG);
    else
      SetPinStateHigh(SegmentG);
  }
  
  void DisplayFraction(const bool visible)
  {
    if(visible)
      SetPinStateHigh(SegmentP);
    else
      SetPinStateLow(SegmentP);
  }

public:  
  
  Multiplexed7Segment(): currInd(0), digitInd(0)
  {
    digits[0] = 1;
    digits[1] = 2;
    digits[2] = 3;
  }

  void Setup7SegmentPins()
  {
    SetPinAsOutput(SegmentA);
    SetPinAsOutput(SegmentB);
    SetPinAsOutput(SegmentC);
    SetPinAsOutput(SegmentD);
    SetPinAsOutput(SegmentE);
    SetPinAsOutput(SegmentF);
    SetPinAsOutput(SegmentG);
    //SetPinAsOutput(SegmentP);
	
    SetPinAsOutput(Digit1Pin);
    SetPinAsOutput(Digit2Pin);
    SetPinAsOutput(Digit3Pin);
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
    DisplayDigit();
    
    if(currInd == 0)
    {
      SetPinStateLow(Digit1Pin);
      SetPinStateHigh(Digit2Pin);
      SetPinStateHigh(Digit3Pin);
    }
    
    if(currInd == 1)
    {
      SetPinStateHigh(Digit1Pin);
      SetPinStateLow(Digit2Pin);
      SetPinStateHigh(Digit3Pin);
    }
    
    if(currInd == 2)
    {
      SetPinStateHigh(Digit1Pin);
      SetPinStateHigh(Digit2Pin);
      SetPinStateLow(Digit3Pin);
    }

    ++currInd;
    if(currInd > 2)
      currInd = 0;
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

