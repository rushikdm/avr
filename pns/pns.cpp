/*
avr-gcc -Wall -Os -mmcu=atmega8 -o pns.o pns.c
avr-objcopy -j .text -j .data -O ihex pns.o pns.hex
avrdude -p ATmega8 -c arduino -P COM3 -b 19200 -U flash:w:pns.hex:i
*/
#define F_CPU 8000000UL
#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>

#define LOW 0
#define HIGH 1

#define PWMPin   15

#define TwentyMSPin      3
#define BuzzerPin        2
#define CircuitCompletePin   14

#define IncreasePWMPin       4
#define DecreasePWMPin       5

#define DisplayPinA      28
#define DisplayPinB      27
#define DisplayPinC      26
#define DisplayPinD      24
#define DisplayPinE      23
#define DisplayPinF      16
#define DisplayPinG      17
#define DisplayPinP      25

#define DisplayPinCC1    18
#define DisplayPinCC2    19

// RESET 1
// GND   8   22
// VCC   7
// AVCC  20
// AREF  21


volatile uint8_t pwm_op_ind = 0;
const uint16_t pwm_op[] =   {10, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950};
const uint8_t ampereValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 25, 30, 35, 40, 50};

volatile uint8_t currentDigit = 0;
volatile uint8_t currentDigitInd = 0;
volatile uint8_t pwm2_ind = 0;

// Common cathode digit hex values:
uint8_t pbvalues[] = { 0x04, 0x00, 0x08, 0x08, 0x0c, 0x0c, 0x0c, 0x00, 0x0c, 0x0c };
uint8_t pcvalues[] = { 0x3b, 0x18, 0x33, 0x3a, 0x18, 0x2a, 0x2b, 0x38, 0x3b, 0x3a };

void determineDigit();
void displayDigit();
void clearDigitDisplay();

int main(void)
{ 
    SetPinAsOutput(PWMPin);
    SetPinAsOutput(TwentyMSPin);
    SetPinAsOutput(BuzzerPin);
    SetPinAsInput(CircuitCompletePin);
    
    SetPinAsInput(IncreasePWMPin);
    SetPinAsInput(DecreasePWMPin);
    
    SetPinAsOutput(DisplayPinA);
    SetPinAsOutput(DisplayPinB);
    SetPinAsOutput(DisplayPinC);
    SetPinAsOutput(DisplayPinD);
    SetPinAsOutput(DisplayPinE);
    SetPinAsOutput(DisplayPinF);
    SetPinAsOutput(DisplayPinG);
    SetPinAsOutput(DisplayPinP);
    
    SetPinAsOutput(DisplayPinCC1);
    SetPinAsOutput(DisplayPinCC2);
    
    pwm_op_ind = 0;
    
    /*   PWM Initialization start ******/
        // set non-inverting mode
        TCCR1A |= (1 << COM1A1);
        
        // set 10bit phase corrected PWM Mode
        TCCR1A |= (1 << WGM11) | (1 << WGM10);
        
        // set prescaler to 8 and starts PWM
        TCCR1B |= (1 << CS11);
        
        // set PWM duty cycle
        OCR1A = pwm_op[pwm_op_ind];
    
    /*   PWM Initialization end ******/
    
    
    /* Start of Interrupt initialization for pin PD2 (4), PD3(5) */

        GICR = (1 << INT0) | (1 << INT1);// Enable INT0 and INT1
	    MCUCR = (1 << ISC01) | (1 << ISC00) | (1 << ISC11) | (1 << ISC10) ;	// Trigger INT0 & INT1 on rising edge
	    
    /* End of Interrupt initialization for pin PD2 (4), PD3(5) */

    /* Start of initialization for timer in CTC mode */
    
        OCR2 = 16;
    
        TCCR2 |= (1 << WGM21);
        // Set to CTC Mode

        TIMSK |= (1 << OCIE2);
        //Set interrupt on compare match

        TCCR2 |= (1 << CS20) | (1 << CS21) | (1 << CS22);
        // set prescaler to 1024
    
    /* End of initialization for timer in CTC mode */
    
    sei();				//Enable Global Interrupt    
    
    while (1)
    {
      //clearDigitDisplay();
      determineDigit();
      displayDigit();
      
      currentDigitInd++;
      if(currentDigitInd > 1)
        currentDigitInd = 0;
    }  
}

ISR(INT0_vect)
{
    pwm_op_ind++;
    
    if(pwm_op_ind >= 20)
      pwm_op_ind = 0;
      
    OCR1A = pwm_op[pwm_op_ind];
}

ISR(INT1_vect)
{
    if(pwm_op_ind > 0)
      pwm_op_ind--;
    else
      pwm_op_ind = 0;
      
    OCR1A = pwm_op[pwm_op_ind];    
}

ISR(TIMER2_COMP_vect)
{
    if(HIGH == GetPinState(CircuitCompletePin))
    {
      if(pwm2_ind <= 0)
        SetPinStateHigh(TwentyMSPin);
      else
        SetPinStateLow(TwentyMSPin);
    
      if(pwm2_ind <= 8)
        SetPinStateHigh(BuzzerPin);
      else
        SetPinStateLow(BuzzerPin);
  
      pwm2_ind++;
      if(pwm2_ind >= 50)
        pwm2_ind = 0;
    }
    else
    {
      SetPinStateLow(TwentyMSPin);
      SetPinStateLow(BuzzerPin);
    }    
}

void determineDigit()
{
  if(0 == currentDigitInd)
    currentDigit = ampereValues[pwm_op_ind] % 10;
  else
    currentDigit = ampereValues[pwm_op_ind]/10 % 10;
}

void displayDigit()
{
  PORTB = (PORTB & 0b00000011) | (pbvalues[currentDigit] & 0b11111100);
  PORTC = 0b00111111 & pcvalues[currentDigit];
  
  if(currentDigitInd == 0)
  {
    SetPinStateLow(DisplayPinCC1);
    SetPinStateHigh(DisplayPinCC2);
    SetPinStateLow(DisplayPinP);
  }
  else
  {
    SetPinStateHigh(DisplayPinCC1);
    SetPinStateLow(DisplayPinCC2);
    SetPinStateHigh(DisplayPinP);
  }
}

void clearDigitDisplay()
{
  PORTB &= 0b00000011;
  PORTC &= 0b00000000;
}

