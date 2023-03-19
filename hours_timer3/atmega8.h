#ifndef ATMEGA8_H
#define ATMEGA8_H

#include <avr/io.h>

class Port
{
public:
 virtual void SetPinAsInput(const uint8_t pin) = 0;
 virtual void SetPinAsOutput(const uint8_t pin) = 0;
 virtual uint8_t IsPinHigh(const uint8_t pin) = 0;
 virtual void SetPinValueHigh(const uint8_t pin) = 0;
 virtual void SetPinValueLow(const uint8_t pin) = 0;
};

/*
class PortA : public Port
{
public:
 virtual void SetPinAsOutput(const uint8_t pin) override
 {
    DDRA |= (1 << pin);
 }
 
 virtual void SetPinAsInput(const uint8_t pin) override
 {
   DDRA &= ~(1 << pin);
 }
  
 virtual uint8_t IsPinHigh(const uint8_t pin) override
 {
    return (PINA & (1 << pin));
 }
 
 virtual void SetPinValueHigh(const uint8_t pin) override
 {
    PORTA |= (1 << pin);
 }
 
 virtual void SetPinValueLow(const uint8_t pin) override
 {
    PORTA &= ~(1 << pin);
 }
};
*/
class PortB : public Port
{
public:
 virtual void SetPinAsOutput(const uint8_t pin) override
 {
    DDRB |= (1 << pin);
 }
 
 virtual void SetPinAsInput(const uint8_t pin) override
 {
   DDRB &= ~(1 << pin);
 }
  
 virtual uint8_t IsPinHigh(const uint8_t pin) override
 {
    return (PINB & (1 << pin));
 }
 
 virtual void SetPinValueHigh(const uint8_t pin) override
 {
    PORTB |= (1 << pin);
 }
 
 virtual void SetPinValueLow(const uint8_t pin) override
 {
    PORTB &= ~(1 << pin);
 }
};

class PortC : public Port
{
public:
 virtual void SetPinAsOutput(const uint8_t pin) override
 {
    DDRC |= (1 << pin);
 }
 
 virtual void SetPinAsInput(const uint8_t pin) override
 {
   DDRC &= ~(1 << pin);
 }
  
 virtual uint8_t IsPinHigh(const uint8_t pin) override
 {
    return (PINC & (1 << pin));
 }
 
 virtual void SetPinValueHigh(const uint8_t pin) override
 {
    PORTC |= (1 << pin);
 }
 
 virtual void SetPinValueLow(const uint8_t pin) override
 {
    PORTC &= ~(1 << pin);
 }
};

class PortD : public Port
{
public:
 virtual void SetPinAsOutput(const uint8_t pin) override
 {
    DDRD |= (1 << pin);
 }
 
 virtual void SetPinAsInput(const uint8_t pin) override
 {
   DDRD &= ~(1 << pin);
 }
  
 virtual uint8_t IsPinHigh(const uint8_t pin) override
 {
    return (PIND & (1 << pin));
 }
 
 virtual void SetPinValueHigh(const uint8_t pin) override
 {
    PORTD |= (1 << pin);
 }
 
 virtual void SetPinValueLow(const uint8_t pin) override
 {
    PORTD &= ~(1 << pin);
 }
};

const uint8_t _atmega8a_pins[28] =  
{  0x26  // 1  PC6 RESET 
 , 0x30  // 2  PD0
 , 0x31  // 3  PD1
 , 0x32  // 4  PD2
 , 0x33  // 5  PD3
 , 0x34  // 6  PD4
 , 0x00  // 7  VCC
 , 0x00  // 8  GND
 , 0x16  // 9  PB6
 , 0x17  // 10 PB7
 , 0x35  // 11 PD5
 , 0x36  // 12 PD6
 , 0x37  // 13 PD7
 , 0x10  // 14 PB0
 , 0x11  // 15 PB1
 , 0x12  // 16 PB2
 , 0x13  // 17 PB3
 , 0x14  // 18 PB4
 , 0x15  // 19 PB5
 , 0x00  // 20 AVCC
 , 0x00  // 21 AREF
 , 0x00  // 22 GND
 , 0x20  // 23 PC0
 , 0x21  // 24 PC1
 , 0x22  // 25 PC2
 , 0x23  // 26 PC3
 , 0x24  // 27 PC4
 , 0x25  // 28 PC5
};

PortB porta;
PortB portb;
PortC portc;
PortD portd;
Port* _atmega8a_ports[4] = {&porta, &portb, &portc, &portd};

void _get_port_and_pin(const uint8_t iPin, Port*& oPort, uint8_t* oPin)
{
  oPort = 0;
  *oPin = 100;
  
  uint8_t port = _atmega8a_pins[iPin-1] & 0xF0;
  *oPin = _atmega8a_pins[iPin-1] & 0x0F;
  if(*oPin > 7)
    return;

  port = port >> 4;
  if(port > 3)
    return;
  
  oPort = _atmega8a_ports[port];
}

void SetPinAsInput(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, &pin);
  if(pPort)
    pPort->SetPinAsInput(pin);
}

void SetPinAsOutput(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, &pin);
  if(pPort)
    pPort->SetPinAsOutput(pin);
}

uint8_t IsPinHigh(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, &pin);
  if(!pPort)
    return 0;
    
  return pPort->IsPinHigh(pin);
}

void SetPinValueHigh(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, &pin);
  if(pPort)
    pPort->SetPinValueHigh(pin);
}

void SetPinValueLow(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, &pin);
  if(pPort)
    pPort->SetPinValueLow(pin);
}

#endif
