#ifndef MY_AVR_UTILS
#define MY_AVR_UTILS

#define LOW 0
#define HIGH 1
/*
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
enum ReleaseType
{
	NO_PRESS=0,
	SIMPLE_PRESS,
	LONG_PRESS
};

class Button
{
	uint16_t press_count;
	uint16_t release_count;
	ReleaseType lastEvent;
	
	public:
	Button()
	{
		press_count = 0;
		release_count = 0;
		lastEvent = NO_PRESS;
	}
	
	ReleaseType process(const bool high)
	{
		if(high)
		{
			if(press_count < 10000)
				++press_count;
			release_count = 0;
		}
		else
		{
			if(release_count < 10000)
				++release_count;
			press_count = 0;
		}
		
		if(press_count > 100)
			lastEvent = SIMPLE_PRESS;
		if(press_count > 800)
			lastEvent = LONG_PRESS;

		if(release_count > 100)
		{
			ReleaseType temp = lastEvent;
			lastEvent = NO_PRESS;
			return temp;
		}
		
		return NO_PRESS;
	}
};

#endif
