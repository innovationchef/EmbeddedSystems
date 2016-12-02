//increase the voltage at pin @1Volt/2sec

#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>

void initialize()
{
	DDRB = 255;
}
void pwm()
{
	TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11)|(1<<WGM10);	
	//Clear OC1A/OC1B on compare match   (inverting mode)
	//PWM phase correct 10 bit
	TCCR1B = (1<<WGM12)|(1<<CS10);		// No prescaling and CTC mode
}
void main()
{
	initialize();
	pwm();
	while(1)
	{
		PORTB = (1<<PORTB1);
		
		for( OCR1A = 0; OCR1A<1024; OCR1A = OCR1A + 204) 
			_delay_ms(2000);				// increase ocra by 1 volt in every 1 second
	}

}