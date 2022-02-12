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

