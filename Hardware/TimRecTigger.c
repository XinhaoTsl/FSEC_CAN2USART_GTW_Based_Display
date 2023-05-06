#include "TimRecTigger.h"

int TimCount12;
int TimCount13;
extern long TimRec[100];

void TimRecTigger_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line12 | EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;        //EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);	
}
	

/* Timer delay config */
void TimRecTimer3Delay_Config(int arr, int psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period = arr;  
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 			/* 10ms time span */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_ITConfig( TIM3, TIM_IT_Update ,ENABLE ); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	TIM_Cmd(TIM3, ENABLE);
}

void TimRecTimer4Delay_Config(int arr, int psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period = arr;  
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 			/* 10ms time span */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	
	TIM_ITConfig( TIM4, TIM_IT_Update ,ENABLE ); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	TIM_Cmd(TIM4, ENABLE);
}



void EXTI15_10_IRQHandler(void)
{

	OLED_ShowString(3,1,"ET");
	if (EXTI_GetITStatus(EXTI_Line12) == SET){
		TimRecTimer3Delay_Config(100 - 1, 7200 - 1);
	}
                        
	if ((EXTI_GetITStatus(EXTI_Line13) == SET)){
		TimRecTimer4Delay_Config(100 - 1, 7200 - 1);
	}

	EXTI_ClearITPendingBit(EXTI_Line12 | EXTI_Line13);
}


/* Start & Recorder commend */
void TIM3_IRQHandler(void) 
{
	extern int TimCount12;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
		{	
			OLED_ShowString(3,3,"_line12");
			TimCount12++;
			if(TimCount12 > 100) TimCount12 = 0;
			TIM_SetCounter(TIM3,0);	   
			TIM_Cmd(TIM3, DISABLE);  
		}
	}	
	
	OLED_ShowNum(4,1,TimCount12,3);	
	
	/* Event Processing */
	if(TimCount13 == 0){
		if(TimCount12 == 1)	/* Mode initation */
			Screen_TimerRecCmd(USART2, (char*)"Init");
		
		else if(TimCount12 == 2)
			Screen_TimerRecCmd(USART2, (char*)"Start");
		
		else if(TimCount12 > 2 && TimCount12 < 50)
			Screen_TimerRecCmd(USART2, (char*)"Rec");
			
		else if(TimCount12 == 50){
			TimCount12 =2 ;			
			Screen_TimerRecCmd(USART2, (char*)"Reset");	
		}
	}
	
	if(TimCount13 == 1 || TimCount13 == 2){
		if(TimCount12 > 1 && TimCount12 < 49){
			TimCount13 = 0;			
			Screen_TimerRecCmd(USART2, (char*)"Start");

		}
	}
	
	if(TimCount13 == 3){
		if(TimCount12 == 2){
			TimCount13 = 0;			
			Screen_TimerRecCmd(USART2, (char*)"Start");
		}
	}

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
}


/* Purse & Reset commend */
void TIM4_IRQHandler(void) 
{
	extern int TimCount13;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
	{		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))
		{	
			OLED_ShowString(3,3,"_line13");
			TimCount13++;
			if(TimCount13 > 100) TimCount13 = 0;
			TIM_SetCounter(TIM4,0);	   
			TIM_Cmd(TIM4, DISABLE);  
		}
	}	
	
	OLED_ShowNum(4,6,TimCount13,3);
	
	/* Event Processing */
	if(TimCount12 == 0 || TimCount12 == 1){
		if(TimCount13 == 1){
			TimCount13 = 0;
			TimCount12 = 0;			
			Screen_TimerRecCmd(USART2, (char*)"Exit");
		}	
		
		if(TimCount13 == 4){
			TimCount12 = 0;		
			TimCount13 = 0;
			Screen_TimerRecCmd(USART2, (char*)"Exit");
		}
	}
	
	if(TimCount12 > 1 && TimCount12 < 49){
		if(TimCount13 == 1){
			Screen_TimerRecCmd(USART2, (char*)"Pause");
		}
		
		else if(TimCount13 == 2){			/* 单层 Reset 仅清空当前显示数据 */
			Screen_TimerRecCmd(USART2, (char*)"Reset");
		}
	
		else if(TimCount13 == 3){			/* 双层 Reset 清空计时模块 */
			TimCount12 = 1;
			TimerSort(TimCount12, TimCount13, 0);		/* 清空数组 */
			Usart_Printf(USART2, (u8*)"t14.aph=0"); Usart_Cmd_END(USART2);
		}	
	}

	
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  
}
