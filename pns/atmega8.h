#ifndef ATMEGA8_H
#define ATMEGA8_H

/* 
---------------Atmega8 pin layout--------------------

      (RESET)  PC6   1        28  PC5  (ADC5/SCL)
        (RXD)  PD0   2        27  PC4  (ADC4/SDA)
        (TXD)  PD1   3        26  PC3  (ADC3)
       (INT0)  PD2   4        25  PC2  (ADC2)
       (INT1)  PD3   5        24  PC1  (ADC1)
     (XCK/T0)  PD4   6        23  PC0  (ADC0)
               VCC   7        22  GND 
               GND   8        21  AREF
(XTAL1/TOSC1)  PB6   9        20  AVCC
(XTAL2/TOSC2)  PB7  10        19  PB5  (SCK)
         (T1)  PD5  11        18  PB4  (MISO)
       (AIN0)  PD6  12        17  PB3  (MOSI/OC2)
       (AIN1)  PD7  13        16  PB2  (SS/OC1B)
       (ICP1)  PB0  14        15  PB1  (OC1A)

-------------------------------------------------------

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
#define  PIN_B0  14
#define  PIN_B1  15
#define  PIN_B2  16 
#define  PIN_B3  17
#define  PIN_B4  18
#define  PIN_B5  19
#define  PIN_B6   9
#define  PIN_B7  10

#define  PIN_C0  23
#define  PIN_C1  24
#define  PIN_C2  25
#define  PIN_C3  26
#define  PIN_C4  27
#define  PIN_C5  28
#define  PIN_C6   1

#define  PIN_D0   2
#define  PIN_D1   3
#define  PIN_D2   4
#define  PIN_D3   5
#define  PIN_D4   6
#define  PIN_D5  11
#define  PIN_D6  12
#define  PIN_D7  13


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

struct Atmega8
{
  const uint8_t m_pins[28] =  
  {   0x26  // 1  PC6 RESET 
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

  PortB m_portB;
  PortC m_portC;
  PortD m_portD;
  Port* m_ports[3] = {&m_portB, &m_portC, &m_portD};
};

Atmega8 _atmega8;

void _get_port_and_pin(const uint8_t iPin, Port*& oPort, uint8_t& oPin)
{
  oPort = 0;
  oPin = 100;

  if( iPin > 28 || iPin == 7 || iPin == 8 || iPin == 20 || iPin == 21 || iPin == 22 ) 
    return;

  oPin = _atmega8.m_pins[iPin-1] & 0x0F;

  uint8_t port = _atmega8.m_pins[iPin-1] & 0xF0;
  port = port >> 4;
  oPort = _atmega8.m_ports[port-1];
}

void SetPinAsInput(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, pin);
  if(pPort)
    pPort->SetPinAsInput(pin);
}

void SetPinAsOutput(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, pin);
  if(pPort)
    pPort->SetPinAsOutput(pin);
}

uint8_t IsPinHigh(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, pin);
  if(!pPort)
    return 0;
    
  return pPort->IsPinHigh(pin);
}

void SetPinValueHigh(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, pin);
  if(pPort)
    pPort->SetPinValueHigh(pin);
}

void SetPinValueLow(const uint8_t iPin)
{
  Port* pPort = 0;
  uint8_t pin = 100;
  _get_port_and_pin(iPin, pPort, pin);
  if(pPort)
    pPort->SetPinValueLow(pin);
}

#endif
