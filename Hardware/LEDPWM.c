#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>


void LedPwm_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); // 为复用开启 AFIO 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 重映射配置 部分重映射1
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;                        // 使用复用 这里需改为 GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_InternalClockConfig(TIM2);
	
	// 时基单元配置
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割 滤波器相关 不设置也可以
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;                  // ARR 数值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;               // PSC 数值 实现 1s 的定时
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;  
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// PWM 模式配置
	
	TIM_OCStructInit(&TIM_OCInitStruct);                          // 结构体自动赋初始值函数 避免编译出错
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 10;                              /* CCR 寄存器配置数 */
																																/* 该值/ARR/PSC 三者共同定 PWM 占空比）*/
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);

}

void PWM_SetARR (uint16_t ArrVal)
{
	TIM_SetAutoreload(TIM2, ArrVal);

}

