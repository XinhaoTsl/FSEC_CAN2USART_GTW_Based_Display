#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

uint32_t LS_input;

void LightSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}

uint32_t LS_inputstate(void)
{
	LS_input = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	return LS_input;
	
}
