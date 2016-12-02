/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h" //属于CMSIS（微控制器软件接口标准）的DPAL（设备外设访问层），包含ST系列处理器所有外设寄存器定义、位定义和不同容量的内存映射

/* Private typedef -----------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;//
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

GPIO_InitTypeDef GPIO_InitStructure;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_TimeBaseInitTypeDef TIM_HALLTimeBaseInitStructure;
TIM_ICInitTypeDef       TIM_HALLICInitStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
TIM_OCInitTypeDef       TIM_HALLOCInitStructure;
TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;

uint16_t CCR1_Val = 32767;
uint16_t CCR2_Val = 24575;
uint16_t CCR3_Val = 16383;
uint16_t CCR4_Val = 8191;
extern __IO uint32_t step;
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM2_Configuration(void);

void Delay(u32 n)
{

while(n--);

}
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{

//u16 i=0;
/* System Clocks Configuration */
RCC_Configuration();
/* GPIO Configuration */
GPIO_Configuration();

    Delay(0xffffff);
    Delay(0xffffff);

/* NVIC Configuration */
NVIC_Configuration();
// Timer configuration in Clear on capture mode
TIM2_Configuration();

  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 4800;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 240;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 240;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 240;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);
TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Trigger);

  /* Automatic Output enable, Break, dead time and lock configuration*/
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStructure.TIM_DeadTime = 1;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;

  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

  TIM_CCPreloadControl(TIM1, ENABLE);

  TIM_ITConfig(TIM1, TIM_IT_COM, ENABLE);

TIM_SelectCOM(TIM1, ENABLE );

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);




//  GPIO_ResetBits(GPIOB,GPIO_Pin_1);

  GPIO_SetBits(GPIOB,GPIO_Pin_1);

  step = GPIO_ReadInputData(GPIOA)&0X7;



 switch(step)
{

case 1:     /*  Channel1 configuration *///C+,A-
     TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
     TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

      /*  Channel2 configuration */
     TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1 );
     TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

  /*  Channel3 configuration */
 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
     TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);
 step= 5;
 break;

case 2:        /*  Channel1 configuration */// A+,B-
     TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
     TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);

      /*  Channel2 configuration */
     TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1 );
     TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
     TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

  /*  Channel3 configuration */
 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);
 step=3;
 break;

case 3:       /*  Channel1 configuration */   //C+,B-
     TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
     TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);

      /*  Channel2 configuration */
     TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1 );
     TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

  /*  Channel3 configuration */
 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
     TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);
 step=1;
 break;

case 4:     /*  Channel1 configuration */  //B+,C-
     TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
     TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
     TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

      /*  Channel2 configuration */
     TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1 );
     TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

  /*  Channel3 configuration */
 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
 step=6;
 break;


case 5:     /*  Channel1 configuration */ //B+,A-
     TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
     TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
     TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

      /*  Channel2 configuration */
     TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1 );
     TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

  /*  Channel3 configuration */
 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);
 step=4;
 break;

case 6:     /*  Channel1 configuration *///A+,C-
     TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
     TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

      /*  Channel2 configuration */
     TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1 );
     TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
     TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

  /*  Channel3 configuration */
 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
     TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
 step=2;
 break;

default:
 break;

}

  while (1)
  {

}

}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);//使用外部晶振

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();//等待外部晶振起振

  if (HSEStartUpStatus == SUCCESS)//起振成功，则进行如下设置
  {

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//预取指缓存使能

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);//设置FLASH存储器延时2个时钟周期数（设置代码延时值）

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);//设置AHB（系统总线，用于高性能模块之间的连接）时钟=系统时钟

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);//设置高速APB(APB2)时钟为AHB时钟

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);//设置低速APB(APB1)时钟为 AHB时钟，注意最大不能超36MHz

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//设置PLL（锁相环-使电路上的时钟和某一外部时钟的相位相同）时钟源为外部晶振频率，倍频系数为9

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);//使能PLL

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)//等待指定的 RCC 标志位设置成功 等待PLL初始化成功
    {

}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//设置系统时钟为PLL时钟

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)//等待PLL成功用作于系统时钟的时钟源
    {

}


}

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//开启TIM2时钟
  /* TIM1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

}

/**
  * @brief  Configure the TIM1 Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{


     /* Configure PA.00,01 ,02 as Hall sensors input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOB Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* GPIOB Configuration: Current_SENSE pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* GPIOB Configuration: Current_SENSE pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


}


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{


    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitTypeDef NVIC_InitHALLStructure;


  #ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

  /* Enable the TIM1 Interrupt */
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM17_IRQn;
#else
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;
#endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

      /* Enable the HALL_TIMER IRQChannel*/
    NVIC_InitHALLStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitHALLStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitHALLStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitHALLStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitHALLStructure);


}
//初始化TIM2定时器
void TIM2_Configuration(void)
{

    TIM_DeInit(TIM2);//将TIM2寄存器值重设为缺省值

    TIM_TimeBaseStructInit(&TIM_HALLTimeBaseInitStructure);//为结构体内每个参数填入缺省值
    // Set full 16-bit working range
    TIM_HALLTimeBaseInitStructure.TIM_Period = 0xFFFF;//自动装载寄存器为最大值
    TIM_HALLTimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2,&TIM_HALLTimeBaseInitStructure);

    TIM_ICStructInit(&TIM_HALLICInitStructure);
    TIM_HALLICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_HALLICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
    TIM_HALLICInitStructure.TIM_ICSelection = TIM_ICSelection_TRC;  //增加TRC的选择
    TIM_HALLICInitStructure.TIM_ICFilter = 0x0B;
    TIM_ICInit(TIM2,&TIM_HALLICInitStructure);

    TIM_OCStructInit(& TIM_HALLOCInitStructure);
    TIM_HALLOCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//PWM模式2
TIM_HALLOCInitStructure.TIM_Pulse = 10;
TIM_HALLOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_HALLOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM2,&TIM_HALLOCInitStructure);//OC2通道配置

    // Force the HALL_TIMER prescaler with immediate access (no need of an update event)
    //注意！！！TIM2的时钟来自于APB1的一个倍频器
    //如果APB1预分频系数为1时，TIMx_CLK= APB1_CLK。否则TIMx_CLK= 2*APB1_CLK
    //本程序RCC配置部分，APB1的预分频系数为2，因此TIM2的时钟为2*APB1_CLK=2*36MHz=72MHz
    TIM_PrescalerConfig(TIM2, (u16) 0xfffe,TIM_PSCReloadMode_Immediate);//计数器时钟CK_CNT为CK_PSC/(0xfffe+1),即k
    TIM_InternalClockConfig(TIM2);//关闭从模式，TIM2由内部时钟CK_INT驱动

    //Enables the XOR of channel 1, channel2 and channel3
    TIM_SelectHallSensor(TIM2, ENABLE);//TIM2_CR2中TI1S位置1，使能异或，连接到TI1输入

    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1F_ED);       //TIM2_SMCR 输入触发方式改为TI1的边沿检测器TI1F_ED，即异或信号

//OC2REF signal is used as the trigger output (TRGO).
TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_OC2Ref);


  // Enable ARR preload
  TIM_ARRPreloadConfig(TIM4, ENABLE);

TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Reset);//从模式：复位模式，由选定的触发输入上升沿重新初始化计数器

   TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

    // Source of Update event is only counter overflow/underflow
    TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Regular);//只有计数器溢出或下溢才产生更新中断

    // Clear the TIMx's pending flags清除标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update + TIM_FLAG_CC1 + TIM_FLAG_CC2 + \
                  TIM_FLAG_CC3 + TIM_FLAG_CC4 + TIM_FLAG_Trigger + TIM_FLAG_CC1OF + \
                  TIM_FLAG_CC2OF + TIM_FLAG_CC3OF + TIM_FLAG_CC4OF);

    // Selected input capture and Update (overflow) events generate interrupt
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);//允许捕获1中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//允许更新中断

    TIM_SetCounter(TIM2,0);//计数器清零
    TIM_Cmd(TIM2, ENABLE);//启动定时器2

}
