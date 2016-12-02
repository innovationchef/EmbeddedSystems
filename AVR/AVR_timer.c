	//glow LED at an interval of one sec using Timer


	#define F_CPU 20000000UL
	#include <avr/io.h>
	#include <util/delay.h>

	void initialize()
	{
		DDRB = 255;		// SET PORT B AS OUTPUT
		TCCR0A = 0;		//normal operation, OCR0A and OCR0B disconnected
		TCCR0B |= (1<<CS01);	//Prescaling 8 == 2.5 MHz
		TCNT0=0;		//Start timer counter register from zero
	}
	void main()
	{	
		initialize();
		int counter=0;
		PORTB |= (1<<PB1);			
		while(1)
		{
			while(TCNT0<255);		// 2.5 MHz /8 == 9800
			counter++;
			if(counter>=9800)		// if counterr = 9800, 1 sec is complete
			{
				PORTB ^= (1<<PB1);		// after 1 sec toggle PB1 
				TCNT0=0;			// Set the counter register back to zero
			    counter=0;				// Set the counter back to zero
			}
		}
	}