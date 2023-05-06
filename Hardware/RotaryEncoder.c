#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_exti.h>
#include <misc.h>
#include <OLED.h>

uint16_t PinA, PinB;
uint16_t StaPinB;

uint16_t RotEncoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	return StaPinB = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}

void RotEncoder_TIconfig(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line14 | EXTI_Line12;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);	
}

/*
void EXTI15_10_IRQHandler(void)
{
	if ((EXTI_GetITStatus(EXTI_Line12) == SET) && RotEncoder_Init())
	{
		if (RotEncoder_Init())
		PinA++;   	
	}
                        
	else if ((EXTI_GetITStatus(EXTI_Line12) == SET) && !RotEncoder_Init())
	{
		if (!RotEncoder_Init())
		PinB++;
	}

	else if ((EXTI_GetITStatus(EXTI_Line14) == SET))
		PinA = PinB =0;
	
	else
		;
	
	EXTI_ClearITPendingBit(EXTI_Line12 | EXTI_Line14);
}

int32_t RotEncoderNum (void)
{
	int32_t PinNum = PinA - PinB;
	return PinNum;
}

*/

