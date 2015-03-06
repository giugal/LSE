////////////////LSE 2015///////////////////
///////////////Practica 2//////////////////
////////////Giuseppe Galioto///////////////
/////////Jorge Sanjuan Garcia//////////////

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#define SERVO_MIN 100
#define SERVO_MAX 200

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure_1, TIM_OCInitStructure_2;
uint16_t CCR1_Val = 150; //PA6
uint16_t CCR2_Val = 150; //PA7
uint16_t PrescalerValue = 239;  // SystemClock (24MHZ) / 239   (240 valores) --> f_TIMER = 1 MHz

int incr_vel = 5;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_COnfiguration(void);
void GPIO_Configuration(void);
void TIM3_IRQHandler(void);

int main(void)
{

  /* System Clocks Configuration */
  RCC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1999;  //Desde 1MHz hasta 50 Hz
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: */
  TIM_OCInitStructure_1.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure_1.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure_2.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure_2.TIM_OCPolarity = TIM_OCPolarity_High;

  // PWM1 1 Configuration:
  TIM_OCInitStructure_1.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure_1.TIM_Pulse = CCR1_Val;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure_1);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  // PWM1 2 Configuration:
  TIM_OCInitStructure_2.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure_2.TIM_Pulse = CCR2_Val;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure_2);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);

  // TIM3 interruption configuration:
  NVIC_COnfiguration();
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  while (1)
  {}
}

void TIM3_IRQHandler(void)
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==1) //Button not pressed
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0)
		{
			CCR1_Val++;
			TIM_OCInitStructure_1.TIM_Pulse = CCR1_Val;
			TIM_OC1Init(TIM3, &TIM_OCInitStructure_1);
		}
		else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==0)
		{
			CCR1_Val--;
			TIM_OCInitStructure_1.TIM_Pulse = CCR1_Val;
			TIM_OC1Init(TIM3, &TIM_OCInitStructure_1);
		}

		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)==0)
		{
			CCR2_Val++;
			TIM_OCInitStructure_2.TIM_Pulse = CCR2_Val;
			TIM_OC2Init(TIM3, &TIM_OCInitStructure_2);
		}
		else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)==0)
		{
			CCR2_Val--;
			TIM_OCInitStructure_2.TIM_Pulse = CCR2_Val;
			TIM_OC2Init(TIM3, &TIM_OCInitStructure_2);
		}
	}
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==0) //Button pressed
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0)
		{
			CCR1_Val=CCR1_Val+incr_vel;
			TIM_OCInitStructure_1.TIM_Pulse = CCR1_Val;
			TIM_OC1Init(TIM3, &TIM_OCInitStructure_1);
		}
		else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==0)
		{
			CCR1_Val=CCR1_Val-incr_vel;
			TIM_OCInitStructure_1.TIM_Pulse = CCR1_Val;
			TIM_OC1Init(TIM3, &TIM_OCInitStructure_1);
		}

		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)==0)
		{
			CCR2_Val=CCR2_Val+incr_vel;
			TIM_OCInitStructure_2.TIM_Pulse = CCR2_Val;
			TIM_OC2Init(TIM3, &TIM_OCInitStructure_2);
		}
		else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)==0)
		{
			CCR2_Val=CCR2_Val-incr_vel;
			TIM_OCInitStructure_2.TIM_Pulse = CCR2_Val;
			TIM_OC2Init(TIM3, &TIM_OCInitStructure_2);
		}
	}

	//Set limits for both servos:
	if(CCR1_Val >= SERVO_MAX)
		CCR1_Val = SERVO_MAX;
	else if(CCR1_Val <= SERVO_MIN)
		CCR1_Val = SERVO_MIN;

	if(CCR2_Val >= SERVO_MAX)
		CCR2_Val = SERVO_MAX;
	else if(CCR2_Val <= SERVO_MIN)
		CCR2_Val = SERVO_MIN;

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void RCC_Configuration(void)
{
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

void NVIC_COnfiguration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  // No StructInit call in API
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*GPIO configuration for PWM*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIO configuration for 5 buttons:
  	  -A1-A2: Up/Down
  	  -A3: Fast
  	  -A4-A5: Rigth/Left.
  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;  // A1 to A5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {}
}

#endif

