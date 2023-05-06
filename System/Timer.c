#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <misc.h>

uint32_t numplus;
uint32_t NumPlus;

void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//时基时钟源的选择（不写也不一定会出问题）
	TIM_InternalClockConfig(TIM2);
	
	//时基单元配置
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割 滤波器相关 不设置也可以
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;                // ARR 数值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;              // PSC 数值 实现 1s 的定时
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;             //高级定时器涉及到的参数 置零即可
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	//中断配置
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// NVIC
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStruct);
	
	//启动定时器
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		NumPlus++;		
		numplus++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

uint32_t TimCountNum(void)
{
	return numplus;

}

