#define F_CPU 14745600ul

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "r.h"

#define baud 38400
#define ACKN 98

#define PULSE_PIN PL4
#define DIR_PIN PL6
#define HIGH 1
#define LOW 0
volatile int Pulse_Count = 0;

long int count=0,max_count=0;
volatile char data_rxd_flag=0;
volatile unsigned int data;
volatile int runflag = 0;
volatile int flag=0,button;
volatile int crap_flag =0;
void init_bot(void)
{
	DDRB=255;
	TCCR1A|=(1<<COM1A1)|(1<<WGM10);
	TCCR1B|=(1<<WGM12)|(1<<CS10)|(1<<CS12);
	TCCR2A|=(1<<COM2A1)|(1<<WGM20)|(1<<WGM21);
	TCCR2B|=(1<<CS20)|(1<<CS22);
	OCR2A=255;
	OCR1A=255;
}
void init_usart(void)
{
	UBRR1=23;
	UCSR1B|=(1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
	UCSR1C|=(1<<UCSZ11)|(1<<UCSZ10);
}
void USARTWriteChar(char data)
{
	while( !( UCSR1A & (1<<UDRE1)))
	{		
	}
	UDR1 = data;	
}
/****************************************************/
/******************** PB0,PB2,PB6,PB7 ***************/
/****************************************************/
void Move_Front()
{
	PORTB|=0b10110100;
	
}
void Move_Back()
{
	PORTB|=0b01110001;
}
void Move_Right()
{
	PORTB|=0b00110101;
	
}
void Move_Left()
{
	PORTB|=0b11110000;
}
void Move_Stop()
{
	PORTB=0b00000000;//all
}
/****************************************************/
void servo_init()
{
	DDRL = 255;
	TCCR5A|=(1<<COM5B1)|(1<<WGM51)|(1<<WGM50); // Clear on compare match, set OCR5B on TOP || Fast PWM Mode with ICR5 as TOP, Clear on Compare Match
	TCCR5B|=(1<<WGM52)|(1<<CS50)|(1<<CS51); // prescaler = 64
}
void servo_right()
{
	DDRL = 255;
	PORTL &= ~(1<<DIR_PIN);
	TCNT4=0;
	OCR5B = 138;
}
void servo_left()
{
	DDRL = 255;
	PORTL |= 1<<DIR_PIN;
	TCNT4 = 0;
	OCR5B = 138;
}
void servo_stop()
{
	DDRL = 0;
}
/****************************************************/
void actuator1_down(void)
{
	PORTH &= ~(1<<1);		//I1  off
	PORTH |= (1<<0)|(1<<3);		// I2 on, EN on (3 is enable)	
}
void actuator1_up(void)
{
	PORTH &= ~(1<<0);		//I2 off
	PORTH |= (1<<1)|(1<<3);	//I1 ON, EN on
}
void actuator1_stop(void)
{
	PORTH &= ~((1<<0)|(1<<1)|(1<<3)) ;	//I1 off, I2 off
}
void actuator2_up(void)
{
	PORTJ |= (1<<0);
	PORTH &= ~(1<<2);
	PORTH |=(1<<4);
}
void actuator2_down(void)
{
	PORTH |= (1<<2);
	PORTJ &= ~(1<<0);
	PORTH |=(1<<4);
	
}
void actuator2_stop(void)
{
	PORTJ &= ~(1<<0);
	PORTH &= ~(1<<2);
	PORTH &=~(1<<4);

}
/****************************************************/
void init_wrist()
{
	DDRE|= (1<<2)|(1<<4)|(1<<6);
	DDRJ |= (1<<2)|(1<<6);
	DDRH |= (1<<6);
	PORTE&=~((1<<2)|(1<<4)|(1<<6));
	PORTJ &=~((1<<2)|(1<<6));
	PORTJ &=~(1<<6);
}
void rotate_wrist_cw()
{
	PORTE&=~(1<<2);
	PORTE |=(1<<6)|(1<<4);
}
void rotate_wrist_ccw()
{
	PORTE&=~(1<<6);
	PORTE |=(1<<4)|(1<<2);
}
void stop_wrist_rotate()
{
	PORTE&=~(1<<4);
	PORTE |=(1<<6);
	
}
void wrist_up()
{
	PORTH |= (1<<6);
	PORTJ |= (1<<6)	;  //******************************************************************
	PORTJ &=~(1<<2)	;  //******************************************************************
}
void wrist_down()
{
	PORTH |= (1<<6);
	PORTJ &=~ (1<<6);
	PORTJ |= (1<<2);				 //******************************************************************
}
void stop_wrist_ud()
{
	PORTH&=~(1<<6);
	PORTJ |=(1<<6);
}
/****************************************************/
void init_actuators_gripper(void)
{
	DDRH = 0xff;
	PORTH &= ~((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4));
	DDRJ =0xff;
	PORTJ &= ~(1<<0); //J0 off
	DDRE |= (1<<3)|(1<<5)|(1<<7);
	PORTE &= ~((1<<1)|(1<<5)|(1<<7));
}
// Gripper Clutch is L298
void gripper_clutch_open(void)
{
	//E3, E5, E7
	PORTE &=~ (1<<7);
	PORTE |= (1<<5);
}
void gripper_clutch_close(void)
{
	//E3, E5, E7
	PORTE &=~ (1<<5);
	PORTE |= (1<<7);
}
void gripper_stop(void)
{
	PORTE &= ~((1<<1)|(1<<5)|(1<<7));
}
void init_bomb_clutch()
{
	DDRH |=(1<<5)|(1<<7);
	//DDRJ |=(1<<1);
	PORTH &= ~((1<<5)|(1<<7));
	//PORTJ &= ~(1<<1);
}
void bomb_clutch_open(void)
{
	//PORTH |= (1<<5);
	PORTH &= ~(1<<7);
	PORTJ |= (1<<1);
}
void bomb_clutch_close()
{
	PORTH |= (1<<7);
	//PORTH &= ~(1<<5);
	PORTJ &= ~(1<<1);
}
void stop_bomb_clutch()
{
	PORTJ &=~(1<<1);
	PORTH &=~((1<<5)|(1<<7));
}
ISR(USART1_RX_vect)
{
	while(((UCSR1A &(1<<RXC1))!=0) && ((UCSR1A &(1<<UDRE1))==0));
	data=UDR1;
	
	if(data==ACKN && flag==0)
	{
		flag=1;
	}
	else if(flag==1)
	{
		button=data;
		crap_flag =1;
		flag=0;
		data_rxd_flag=1;
	}
}
ISR(TIMER5_OVF_vect)
{
	count++;
	if (count==(max_count*10))
	{
		DDRL=0;
	}
}
void adc_init(void){
	
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);
}
uint16_t read_adc(uint8_t channel){
	ADMUX &= 0xE0;
	ADMUX |= channel&0x07;
	ADCSRB = channel&(1<<3);
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADCW;
}
uint16_t Sensor1, Sensor2;

int main()
{
	cli();
	LCD_INIT(LCD_DISP_ON_CURSOR);
	servo_init();
	init_bot();
	init_usart();
	init_actuators_gripper();
	init_bomb_clutch();
	init_wrist();
	adc_init();
	sei();
	DDRC=255;
	char data_crap[20];
	while(1)
	{
		LCD_HOME_POS();
		LCD_PRINT("Ready yo!!");
		_delay_ms(100);
		
		/*
		if (crap_flag)
		{
			crap_flag =0;
			LCD_HOME_POS();
			 itoa(data,data_crap,10);
			LCD_PRINT(data_crap);
			_delay_ms(100);
		}
		*/
		Sensor1 = read_adc(0);
		Sensor2 = read_adc(1);
		
		//LCD_CLRSCR();
			if(Sensor1 >=128 && Sensor2>=128)
			{
			//     LCD_GOTOXY(0,0);
			 //LCD_PRINT("X");
				USARTWriteChar('x');
			}
			else
			{
				//LCD_GOTOXY(0,0);
			//LCD_PRINT("Y");
			USARTWriteChar('y');
	}			
			
	if(data_rxd_flag)
		{
			switch (button)
			{
				LCD_HOME_POS();
				LCD_PRINT("Ready yo!!");
				_delay_ms(100);
				case 0:
				{
					LCD_HOME_POS();
					LCD_PRINT("Ready yo!!");
					_delay_ms(100);
					Move_Stop();
					servo_stop();
					actuator1_stop();
					actuator2_stop();
					gripper_stop();
					stop_bomb_clutch();
					stop_wrist_rotate();
					stop_wrist_ud();
					data_rxd_flag=0;
					break;
				}
				case 1:
				{
					LCD_HOME_POS();
					LCD_PRINT("forward");
					_delay_ms(100);
					Move_Front();
					data_rxd_flag=0;
					
					break;
				}
				case 2:
				{
					LCD_HOME_POS();
					LCD_PRINT("backward");
					_delay_ms(100);
					Move_Back();
					data_rxd_flag=0;
					break;
				}
				case 3: 
				{
					LCD_HOME_POS();
					LCD_PRINT("right");
					_delay_ms(100);
					Move_Right();
					data_rxd_flag=0;
					break;
				}
				case 4:
				{
					LCD_HOME_POS();
					LCD_PRINT("left");
					_delay_ms(100);
					Move_Left();
					data_rxd_flag=0;
					break;
				}
				case 5: 
				{
					LCD_HOME_POS();
					LCD_PRINT("servo left");
					_delay_ms(100);
					servo_left();
					data_rxd_flag=0;
					break;
				}
				case 6: 
				{ 
					LCD_HOME_POS();
					LCD_PRINT("servo right");
					_delay_ms(100);
					servo_right();
					data_rxd_flag=0;
					break;
				}
				case 7:
				{
						//For clutch of the gripper
						LCD_CLRSCR();
					LCD_HOME_POS();
					LCD_PRINT("Clutch open");
					_delay_ms(100);
					gripper_clutch_open();
					data_rxd_flag=0;
					break;
				}
				case 8:
				{
					LCD_CLRSCR();
					LCD_HOME_POS();
					LCD_PRINT("Clutch close");
					_delay_ms(100);
					gripper_clutch_close();
					data_rxd_flag=0;
					
					break;
				}
				case 9:
				{	//For Actuator UP
					LCD_HOME_POS();
					LCD_PRINT("Actuator 1 up");
					_delay_ms(100);
					actuator1_up();
					data_rxd_flag=0;
					break;
				}
				case 10:
				{
					LCD_HOME_POS();
					LCD_PRINT("Actuator 1 down");
					_delay_ms(100);
					actuator1_down();
					data_rxd_flag=0;
					break;
				}
				case 11:
				{
					LCD_HOME_POS();
					LCD_PRINT("Actuator 2 up");
					_delay_ms(100);
					actuator2_up();
					data_rxd_flag=0;
					break;
				}
				case 12:
				{
					LCD_HOME_POS();
					LCD_PRINT("Actuator 2 down");
					_delay_ms(100);
					actuator2_down();
					data_rxd_flag=0;
					break;
				}
				case 13:
				{
					LCD_HOME_POS();
					LCD_PRINT("bomb clutch open");
					_delay_ms(100);
					bomb_clutch_open();
					data_rxd_flag=0;
					break;
				}
				case 14:
				{
					LCD_HOME_POS();
					LCD_PRINT("bomb clutch close");
					_delay_ms(100);
					bomb_clutch_close();
					data_rxd_flag=0;
					break;
				}
				
				case 15:
				{
					
				break;
				}
				
				case 16:
				{
					
					break;
				}
				
				case 17:
				{
					LCD_HOME_POS();
					LCD_PRINT("wrist up");
					_delay_ms(100);
					wrist_up();
					data_rxd_flag=0;
					break;
				}
				
				case 18:
				{
					LCD_HOME_POS();
					LCD_PRINT("wrist down");
					_delay_ms(100);
					wrist_down();
					data_rxd_flag=0;	
					break;
				}
				
				case 19:
				{
					LCD_HOME_POS();
					LCD_PRINT("wrist CCW");
					_delay_ms(100);
					rotate_wrist_ccw();
					data_rxd_flag=0;
					break;
				}
				
				case 20:
				{
					LCD_HOME_POS();
					LCD_PRINT("wrist CW");
					_delay_ms(100);
					rotate_wrist_cw();
					data_rxd_flag=0;
					break;
				}
				
				
			}				
		}
		_delay_ms(500);
	}

	return 0;
}
 