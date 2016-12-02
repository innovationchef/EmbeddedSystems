
#include <stm32f30x.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f30x_gpio.h>			//	include the gpio file
#include <stm32f30x_rcc.h>			//include the clock library
#include <stm32f30x_tim.h>			// include timer library

//Initializes the output pins for the Timer channels
void pwm_to_motor()
{
	// The Timer 1 channels 1,2 and 3 are connected to PE09 (channel 1), PE11 (channel 2), and PE13 (channel 3)
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 		// GPIO speed - has nothing to do with the timer timing
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		 	// Push-pull
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			 // Setup pull-up resistors
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	// Connect the timer output to the LED pins
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_2); // TIM1_CH1 -> PE09
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_2); // TIM1_CH2 -> PE11
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_2); // TIM1_CH3 -> PE13
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_2); // TIM1_CH3 -> PE13

}

//@brief  Initializes PWM
//@param  pwm_freq: Frequency of the PWM in Hz
//@return period
int initialize_pwm(int pwm_freq)
{
			// Calculates the timing. This is common for all channels
	int original_ticks = 84e6; 								// 84MHz
	int prescaler = 41;
	int available_ticks = original_ticks/(prescaler + 1);  // = 2e6
			// Calculate the period for a given pwm frequency
			//say int pwm_freq = 200; // in Hz
	int pwm_period = available_ticks/pwm_freq;				// 2MHz / 200Hz = 10000

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE );
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(& TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseStructure.TIM_Period = pwm_period - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;            // dont know why..............
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

			// Initialise the timer channels
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			//say period is 500 and duty cycle is 80%. Hence keep pulse = 80% of 500. This will keep pin high for 4/5 part of cycle
	TIM_OCInitStructure.TIM_Pulse = 0; 					// preset pulse width 0..pwm_period
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// Pulse polarity
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// These settings must be applied on the timer 1.
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
// Setup four channels
//We Dont need all four channels but I am initializing all four for to keep things easier in future
	// Channel 1
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	// Channel 2
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	// Channel 3
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	// Channel 4
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);   		// since TIM1 is spcl timer we have to enable this
	TIM_Cmd(TIM1 , ENABLE);
	return pwm_period;
}
// function to set duty cycle
void set_pwm_width_norm(int channel, int pwm_period, float duty_cycle)
{
	int pwm_pulses = pwm_period*(float)duty_cycle;
	switch (channel){
		case 1: TIM_SetCompare1(TIM1, pwm_pulses); break;
		case 2: TIM_SetCompare2(TIM1, pwm_pulses); break;
		case 3: TIM_SetCompare3(TIM1, pwm_pulses); break;
		case 4: TIM_SetCompare4(TIM1, pwm_pulses); break;
	}
}
void main()
{
	pwm_to_motor();
	// Pwm frequency to FIRA motors 2000Hz
	int pwm_period = initialize_pwm(2000);  // 2Mz/PWM_freq
	int i=0;
	while(1)
	{
	//for(i=0; i<998; i++)
	//TIM_SetCompare1(TIM1, i);
	set_pwm_width_norm(1, pwm_period, 0.1);
	set_pwm_width_norm(2, pwm_period, 0.1);
	set_pwm_width_norm(3, pwm_period, 0.1);
	set_pwm_width_norm(4, pwm_period, 0.1);
	Delay(1000000);
	set_pwm_width_norm(1, pwm_period, 0.6);
	set_pwm_width_norm(2, pwm_period, 0.6);
	set_pwm_width_norm(3, pwm_period, 0.6);
	set_pwm_width_norm(4, pwm_period, 0.6);
	Delay(1000000);
	}
}
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
