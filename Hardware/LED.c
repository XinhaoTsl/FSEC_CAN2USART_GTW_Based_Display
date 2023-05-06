#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_7);

}

void LED0_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void LED0_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void LED7_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
}

void LED7_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
}
	
void LED0_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) == 0)
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
	else
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);	
}			

void LED7_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7) == 0)
			GPIO_SetBits(GPIOA, GPIO_Pin_7);
	else
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);	
}		

