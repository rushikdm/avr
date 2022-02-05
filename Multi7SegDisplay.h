#ifndef MULTI7SEGDISPLAY_H
#define MULTI7SEGDISPLAY_H
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

class Pin
{
  volatile uint8_t* port;
  uint8_t pin;
  
public:
  
  Pin(volatile uint8_t* iport, const uint8_t ipin): port(iport), pin(ipin) 
  {
  }
  
  void on() const
  { 
    (*port) |= (1 << pin);  
  }
  
  void off() const
  { 
    (*port) &= ~(1 << pin);
  }
};

class Multi7SegDisplay
{
  const uint8_t segData[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
  uint8_t digits[2] = {1, 2};
  uint8_t digitMInd = 0;
  uint8_t segMInd = 0;
  uint8_t digitInd = 0;
  const Pin segPins[7] = { {&PORTC, 5}, {&PORTC, 4}, {&PORTC, 3}, {&PORTC, 1}, {&PORTC, 0}, {&PORTD, 0}, {&PORTD, 1}};
  const Pin digitPins[2] = { {&PORTB, 5}, {&PORTB, 4}};
  
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
    SetPinAsOutput(SegmentA);
    SetPinAsOutput(SegmentB);
    SetPinAsOutput(SegmentC);
    SetPinAsOutput(SegmentD);
    SetPinAsOutput(SegmentE);
    SetPinAsOutput(SegmentF);
    SetPinAsOutput(SegmentG);
	
    SetPinAsOutput(Digit1Pin);
    SetPinAsOutput(Digit2Pin);
    
    SetPinStateHigh(SegmentA);
    SetPinStateHigh(SegmentB);
    SetPinStateHigh(SegmentC);
    SetPinStateHigh(SegmentD);
    SetPinStateHigh(SegmentE);
    SetPinStateHigh(SegmentF);
    SetPinStateHigh(SegmentG);
    
    SetPinStateLow(Digit1Pin);
    SetPinStateLow(Digit2Pin);
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
  
  void Multiplex()
  {
    segPins[segMInd > 0 ? (segMInd-1) : 6].on();
    
    if(segMInd == 0)
    {
      digitPins[digitMInd].on();
      digitPins[digitMInd > 0 ? (digitMInd-1) : 1].off();
    }
    
    if(present(digits[digitMInd], segMInd))
      segPins[segMInd].off();
    
    ++segMInd;
    if(segMInd > 6)
    {
      segMInd = 0;
      ++digitMInd;
      if(digitMInd > 1)
        digitMInd = 0;
    }
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

