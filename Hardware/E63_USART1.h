#ifndef __E63_USART1_H
#define __E63_USART1_H

#include <stdio.h>
#include "Device/Include/stm32f10x.h"   // Device header
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_can.h>
#include <stm32f10x_exti.h>
#include <misc.h>
#include <OLED.h>
#include <Delay.h>
#include <stm32f10x_dma.h>
#include <string.h>
#include <stdlib.h>

#define TimRecLen 50

extern char UsartIT_RxMsg[50];
extern u8 RxFlag;

typedef struct
{
	u32 USARTx_RCC;
	u32 GPIOx_RCC;
	GPIO_TypeDef* GPIOx;
	u16 USARTx_PinRx;
	u16 USARTx_PinTx;

}USARTx_InitDef;


/* Usable Function Table */

void Usart_InitCongif(USART_TypeDef* USARTx, USARTx_InitDef * USARTx_InitMsg, u32 USARTx_BaudRate);
void NVIC_Config(void);
void Usart1_IT_Config(void);
void Usart2_IT_Config(void);

void USART1_DAM_Config(u32 paddr, u32 maddr, u16 len);
void USART2_DAM_Config(u32 paddr, u32 maddr, u16 len);

char* Int2Str(USART_TypeDef* USARTx, int num);
void Usart_Printf(USART_TypeDef* USARTx, u8 *str);
void Usart_Send_Byte(USART_TypeDef* USARTx, char Usart_Byte);
void Usart_Send_Str(USART_TypeDef* USARTx, u8 *str);
void Usart_Send_Int2Str(USART_TypeDef* USARTx, int num);
void Usart_Cmd_END(USART_TypeDef* USARTx);
void Screen_NumCmd(USART_TypeDef* USARTx, u8 *ScreenNumCmd, int num);
void Screen_u8NumCmd(USART_TypeDef* USARTx, u8 *ScreenNumCmd, u8 num);

void Screen_TxtNumCmd(USART_TypeDef* USARTx, u8 *ScreenNumCmd, int num);      					  //���� ���ת�� ʾ��ָ���
void Screen_StateRing(USART_TypeDef* USARTx, u8 *ScreenNumCmd, int num);									//̤�忪�� ���ת�� ָʾ��
void Screen_BattState(USART_TypeDef* USARTx, int UpperVolt, int LowerVolt);   					  //���״̬��ʾ����
void Screen_GearMsg(USART_TypeDef* USARTx, u8 *ScreenNumCmd, int GearNum, int TsVolt);    //��λ״̬������Ϣ����
int Screen_MotorWarningJudgement(USART_TypeDef* USARTx, char* WhichMotor, int StaNum);    //���������Ϣ��ʾ����

int Screen_TimerRecCmd(USART_TypeDef* USARTx, char* CmdMsg);						//��ʱ��ģʽ
int TimerRecord(USART_TypeDef* USARTx, int* TimRec, int BitLen); 				//Ȧ�ټ�¼��
int TimerDisp(USART_TypeDef* USARTx);																		//����ʱģ����ʾ����
int TimerHide(USART_TypeDef* USARTx);																		//����ʱģ�����غ���
int TimerSort(int countss, int countpr, long TimRec);										//��ʱ������
	
#endif
