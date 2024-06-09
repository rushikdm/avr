#ifndef AMPERE_RANGE_H
#define AMPERE_RANGE_H

#define AMPERE_RANGE 20

class PNSRange
{
  public: 
  	PNSRange() : m_index(0) {}
  	
  	void increase()
  	{
  		++m_index;
  		if(m_index >= AMPERE_RANGE)
  			m_index = 0;
  	}
  	
  	void decrease()
  	{
  		if(m_index > 0)
  			--m_index;
  		else
  			m_index = AMPERE_RANGE-1;
  	}
  	
  	uint8_t getAmpereValue()
  	{
  		return m_ampere_values[m_index];
  	}
  	
  	uint8_t getAmpereDigit(const bool first_digit)
  	{
			if(first_digit)
				return m_ampere_values[m_index] % 10;

			return m_ampere_values[m_index]/10 % 10;
  	}
  	
  	uint16_t getPWMValue()
  	{
  		return m_pwm_values[m_index];
  	}
  	
  private:
  	const uint8_t m_ampere_values[20] = {1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  12,  14,  16,  18,  20,  25,  30,  35,  40, 50};
  	const uint16_t m_pwm_values[20] =   {10, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950};
  	uint8_t m_index;
};

#endif

