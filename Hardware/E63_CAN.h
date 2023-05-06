#ifndef __CAN_H
#define __CAN_H	 
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_can.h>
#include <stm32f10x_exti.h>
#include <misc.h>
#include <OLED.h>
#include <Delay.h>
#include <E63_USART1.h>

#define CAN_INT_ENABLE	0	

#define tsjw	CAN_SJW_1tq	
#define tbs1	CAN_BS1_8tq	
#define tbs2	CAN_BS2_7tq	
#define brp		9

/* Critical Variable */
/*
VehSpd = 0;
Gear = 0;
PedalSta = 0;
MotSpdL = 0;
MotSpdR = 0;
BatMaxVolt = 0;
BatMinVolt = 0;
TsVolt = 0;
McuRdyL = 0;
McuRdyR = 0;
*/


u8 CAN1_Configuration(void);
void CAN1_FilterConfig(u32 CAN_FilterId, u32 CAN_Musk, u8 CAN_FilterNumber, u16 CAN_FilterFIFOAssignment);
void CAN1_ITConfig(void);
void USB_LP_CAN1_RX0_IRQHandler(void);
void CAN1_SCE_IRQHandler(void);

u8 CAN_Send_Msg(u32 MsgId, u8* msg,u8 len);
u8 CAN_Receive_Msg(u8 *ptr);

int MotorolaDataCov(int num_2B, int num_1B, int offset, double gain);   // Motorla –≠“È processing
		 				    
#endif
