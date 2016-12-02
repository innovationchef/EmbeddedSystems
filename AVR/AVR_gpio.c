#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>		/// lib for delay function

void initialize()
{
	DDRB = 255;		//initialize Port B
}
void main()
{
	initialize();
	while(1){
		PORTB^=(1<<PB4);		// Toggle PB4
		_delay_ms(500);
		PORTB^=(1<<PB5);
		_delay_ms(500);		// Toggle PB4

	}
}