//after the interrupt is called, both leds glows together for 3 sec
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 2000000UL
#include <util/delay.h>

ISR(INT0_vect)
{
	PORTB ^= (1<<PB4)|(1<<PB5);
}
void initialize()
{
	DDRB = 255;
}
void int_ex_intrrupt()
{
	EICRA = (1<<ISC01)|(1<<ISC00);		// interrupt on rising edge  
	EIMSK = (1<<INT0);			// int 0 external interrupt on pin PD2
}
void main()
{
	initialize();
	int_ex_intrrupt();
	sei();
	while(1)
	{

	}
}