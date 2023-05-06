#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_exti.h>
#include <misc.h>

uint16_t CountNum11;

void Count_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	EXTI_InitTypeDef EXTI_InitStruc;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//配置 AFIO外部中断引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	//EXTI 结构体初始化配置
	EXTI_InitStruc.EXTI_Line = EXTI_Line14;
	EXTI_InitStruc.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruc.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruc.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_InitStruc);
	
	//配置 NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel= EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
		
	NVIC_Init(&NVIC_InitStruct);
	
}

uint16_t CountNum(void)
{
	return CountNum11;
}

 
// 中断函数内执行内容定义
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountNum11 ++;
		EXTI_ClearITPendingBit(EXTI_Line11);
	}

}
 
