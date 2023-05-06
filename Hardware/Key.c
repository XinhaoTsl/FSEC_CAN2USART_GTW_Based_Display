#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <Delay.h>

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}

uint8_t Key_Getnum1(void)
{
	uint8_t KeyNum1;
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		/*Delay_ms(20);
				
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
					Delay_ms(20);*/
		
		KeyNum1 = 1;
	}
	else
			KeyNum1 = 0;
	
	return KeyNum1;
}

uint8_t Key_Getnum2(void)
{
	uint8_t KeyNum11;
	KeyNum11 = 0;	
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
				KeyNum11 = 2;
			Delay_ms(20);
	}
	else
		//Delay_ms(5);
		KeyNum11 = 0;
		
	return KeyNum11;
}
