

#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
long count;
ISR(TIMER0_OVF_vect)		// interrupt service routine for timer 0
{
	count++;			
	if(count >= 235290)
	{
		PORTB ^= (1<<PB1);
	}
}
void initialize()
{
	DDRB = 255;		//Port B as output
	TCCR0A = 255;		//set ocr1a/ocr1b on compare match + 10 bit pwm pwm mode
	TCCR0B = (1<<CS00);	//no prescaling
	TCNT0 = 0;		//initialize counter register at 0
	TIMSK0 = (1<<TOIE0);	// overflow interrupt enablle
	TIFR0 = (1<<TOV0);		// set timer counter overflow flag 1
	
}
void main()
{
	initialize();
	sei();			// enable global interrupt
	while(1){
	}
}