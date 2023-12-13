/*
@author: Rushikesh Keshvaro Deshmukh
@date : 13-Dec-2023
avr-gcc -Wall -Os -mmcu=atmega8 -o led.o led.c 
avr-objcopy -j .text -j .data -O ihex led.o led.hex 
avrdude -p ATmega8 -c USBasp -P COM3 -b 19200 -U flash:w:led.hex:i 
*/
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
  DDRB != 0x01;
  
  while(true)
  {
    PORTB ^= 0x01;
    _delay_ms(1000);
  }

  return 0;
}

