#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <misc.h>
#include <OLED.h>

uint16_t ARRval = 50000-1;
uint16_t PSCval = 7200-1;                 //5秒翻转一次 (50000/(72e6/72e2)) 避免溢出
uint8_t Tim3_Capture_sta = 0;
uint16_t FunCallSta;
uint16_t FunCallTimes;
uint16_t Tim3_Capture_val;

void WaveCapture_Init(void)
{

	
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        // 使用 PA6  为复用开启 AFIO 时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	 
	// 使能输入引脚 PA6
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;                   // PA6 配置为下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
	// 配置时基单元
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ARRval;
	TIM_TimeBaseInitStruct.TIM_Prescaler = PSCval;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	// 输入捕获模式配置
	TIM_ICInitStruct.TIM_Channel =  TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;     //上升沿 下降沿 配置
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;

	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	// TIM_CCxNCmd(TIM3, TIM_Channel_1, TIM_CCxN_Enable);      //输入捕获通道使能
	
	// NVIC config
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	// 使能 Timer
	TIM_Cmd(TIM3, ENABLE);
	FunCallTimes = 1;
	FunCallSta = 0;
	
	// 使能内部中断通道
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);         // 使能输入捕获中断通道
	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);      // 使能溢出中断通道
	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		if(FunCallSta == 0)
		{
			Tim3_Capture_val = TIM_GetCounter(TIM3); 
			FunCallSta = 1;
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);
			FunCallTimes++;                                     //记触发次数/屏蔽首次非正常Count数值
		
			if (FunCallTimes > 10000)
				FunCallTimes = 2;
			
			TIM_SetCounter(TIM3, 0);
			//OLED_ShowString(2,1,"YES");//测试
		}
		else if (FunCallSta == 1)
		{
			FunCallSta = 0;
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising);
			//OLED_ShowString(1,10,"YES");//测试
		}
	
	}
		
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
