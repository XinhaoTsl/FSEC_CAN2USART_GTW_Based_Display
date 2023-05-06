#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <misc.h>
#include <OLED.h>

uint16_t ARRval = 50000-1;
uint16_t PSCval = 7200-1;                 //5�뷭תһ�� (50000/(72e6/72e2)) �������
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        // ʹ�� PA6  Ϊ���ÿ��� AFIO ʱ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	 
	// ʹ���������� PA6
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;                   // PA6 ����Ϊ����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
	// ����ʱ����Ԫ
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ARRval;
	TIM_TimeBaseInitStruct.TIM_Prescaler = PSCval;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	// ���벶��ģʽ����
	TIM_ICInitStruct.TIM_Channel =  TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;     //������ �½��� ����
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;

	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	// TIM_CCxNCmd(TIM3, TIM_Channel_1, TIM_CCxN_Enable);      //���벶��ͨ��ʹ��
	
	// NVIC config
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	// ʹ�� Timer
	TIM_Cmd(TIM3, ENABLE);
	FunCallTimes = 1;
	FunCallSta = 0;
	
	// ʹ���ڲ��ж�ͨ��
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);         // ʹ�����벶���ж�ͨ��
	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);      // ʹ������ж�ͨ��
	
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
			FunCallTimes++;                                     //�Ǵ�������/�����״η�����Count��ֵ
		
			if (FunCallTimes > 10000)
				FunCallTimes = 2;
			
			TIM_SetCounter(TIM3, 0);
			//OLED_ShowString(2,1,"YES");//����
		}
		else if (FunCallSta == 1)
		{
			FunCallSta = 0;
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising);
			//OLED_ShowString(1,10,"YES");//����
		}
	
	}
		
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
