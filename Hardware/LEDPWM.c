#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>


void LedPwm_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); // Ϊ���ÿ��� AFIO ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// ��ӳ������ ������ӳ��1
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;                        // ʹ�ø��� �������Ϊ GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_InternalClockConfig(TIM2);
	
	// ʱ����Ԫ����
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ� �˲������ ������Ҳ����
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;                  // ARR ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;               // PSC ��ֵ ʵ�� 1s �Ķ�ʱ
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;  
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// PWM ģʽ����
	
	TIM_OCStructInit(&TIM_OCInitStruct);                          // �ṹ���Զ�����ʼֵ���� ����������
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 10;                              /* CCR �Ĵ��������� */
																																/* ��ֵ/ARR/PSC ���߹�ͬ�� PWM ռ�ձȣ�*/
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

