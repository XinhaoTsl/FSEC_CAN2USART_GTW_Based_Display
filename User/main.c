#include "stm32f10x.h"
#include <stdio.h>
#include <LEDPWM.h>
#include <OLED.h>
#include <delay.h>
#include <stm32f10x_tim.h>
#include <WaveCapture.h>
#include <stm32f10x_gpio.h>
#include <LightSensor.h>
#include <stm32f10x_exti.h>
#include <Count.h>
#include <E63_CAN.h>
#include <Delay.h>
#include <stm32f10x_can.h>
#include <E63_USART1.h>
#include <string.h>
#include "TimRecTigger.h"

u8 USART1_RxMsg1[35] = {0};
u8 USART2_RxMsg1[35] = {0};

int TimLen = 0;

u32 CAN1Tx1_Id = 0x0fd1e638;
u32 CAN1Tx2_Id = 0x0fd2e639;
u32 CAN1_FId1 = 0x1fd5163a;
u32 CAN1_Musk1 = 0xFFFFFFF0;
u32 CAN1_FId2 = 0x1fd7163b;
u32 CAN1_Musk2 = 0xFFFFFFF0;

/* Critical Variable */
int VehSpd;
int Gear;
int PedalSta;
int MotSpdL;
int MotSpdR;
int BatMaxVolt;
int BatMinVolt;
int TsVolt;
int McuRdyL;
int McuRdyR;

int MotWarnL_Disp, MotWarnR_Disp, SysWarn_Disp;
//long TimRec[100];

extern int McuRdyR;
extern int TimCount12;
extern int TimCount13;

int CAN_RxMsg1[8] = {0};
int CAN_RxMsg2[8] = {0};

u8 Tx1Mbox, Tx2Mbox;
u8 TxMsg1[8] = {0};   
	
int main (void){

	// USART TX Msg
	//u8 UTxMsg1[8] = {0};
	
	// Hardware Config
	CAN1_Configuration(); 		
	CAN1_FilterConfig(CAN1_FId1, CAN1_Musk1, 0, CAN_Filter_FIFO0);			//FIFO_0 滤波设置
	CAN1_FilterConfig(CAN1_FId2, CAN1_Musk2, 2, CAN_Filter_FIFO0);			//FIFO_0 滤波配置
	CAN1_ITConfig();
	OLED_Init();
	
	// USART1 Config
	USARTx_InitDef  USARTx_InitMsg;
	
	USARTx_InitMsg.GPIOx = GPIOA;
	USARTx_InitMsg.GPIOx_RCC = RCC_APB2Periph_GPIOA;
	USARTx_InitMsg.USARTx_PinRx = GPIO_Pin_10;
	USARTx_InitMsg.USARTx_PinTx = GPIO_Pin_9;
	USARTx_InitMsg.USARTx_RCC = RCC_APB2Periph_USART1;
	Usart_InitCongif(USART1, &USARTx_InitMsg, 115200);
	
	Usart1_IT_Config();																					   // 闲置中断模式
	USART1_DAM_Config((u32)&USART1->DR, (u32)USART1_RxMsg1, 35);	 // USART1 DMA Config
	
	// USART2 Config
	USARTx_InitMsg.USARTx_PinRx = GPIO_Pin_3;	
	USARTx_InitMsg.USARTx_PinTx = GPIO_Pin_2;	
	USARTx_InitMsg.USARTx_RCC = RCC_APB1Periph_USART2;
	Usart_InitCongif(USART2, &USARTx_InitMsg, 115200);	

	Usart2_IT_Config();																					   
	USART2_DAM_Config((u32)&USART2->DR, (u32)USART2_RxMsg1, 35);
	
	// TimerRecoder Config
	TimRecTigger_Init();
	TimCount12 = 0; TimCount13 = 0;	

	Screen_TimerRecCmd(USART2, (char*)"Reset");
	Screen_TimerRecCmd(USART2, (char*)"Exit");

	TimerSort(1, 3, 0);	
	

	
	while(1){
		//OLED_ShowString(1, 1, "Universal Test");				
		
		/* CAN Tx */
		Tx1Mbox = CAN_Send_Msg(CAN1Tx1_Id, TxMsg1,8);
		
		/* USART Tx */
		//Screen_NumCmd(USART2, "n0.val", 6);
		
		/* CAN Tx Safity Chack*/
		OLED_ShowNum(1,14,((CAN_TransmitStatus(CAN1, Tx1Mbox) == CAN_TxStatus_Ok) && (CAN_TransmitStatus(CAN1, Tx2Mbox) == CAN_TxStatus_Ok)),1);
		OLED_ShowNum(1,15,(CAN_TransmitStatus(CAN1, Tx1Mbox) == CAN_TxStatus_Ok),1);
		OLED_ShowNum(1,16,(CAN_TransmitStatus(CAN1, Tx2Mbox) == CAN_TxStatus_Ok),1);
		
		if(!((CAN_TransmitStatus(CAN1, Tx1Mbox) == CAN_TxStatus_Ok) && (CAN_TransmitStatus(CAN1, Tx2Mbox) == CAN_TxStatus_Ok))){
			OLED_ShowString(1,1,"CAN offLine");
			Usart_Printf(USART2, (u8*)"t10.aph=127"); Usart_Cmd_END(USART2);
			Usart_Printf(USART2, (u8*)"t10.txt=\"CAN comumunication LOST\""); Usart_Cmd_END(USART2);
		}		
		else {
			OLED_ShowString(1,1,"CAN onLine");
			Usart_Printf(USART2, (u8*)"t10.aph=0"); Usart_Cmd_END(USART2);
		}
		
		/* Systematic Error Check */
		if(MotWarnL_Disp == 0){
			Usart_Printf(USART2, (u8*)"t9.aph=0"); Usart_Cmd_END(USART2);}
		else if(MotWarnL_Disp == 1){
			Usart_Printf(USART2, (u8*)"t9.aph=127"); Usart_Cmd_END(USART2);}
		
		if(MotWarnR_Disp == 0){
			Usart_Printf(USART2, (u8*)"t8.aph=0"); Usart_Cmd_END(USART2);}
		else if(MotWarnR_Disp == 1){
			Usart_Printf(USART2, (u8*)"t8.aph=127"); Usart_Cmd_END(USART2);}

			
		/* Sending/Update Time Span*/
		Delay_ms(100);	

	}
}

/* CAN Rx IT function */
void USB_LP_CAN1_RX0_IRQHandler(void){
	
    CanRxMsg RxMessage;
    CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);	
	  //CAN_Receive(CAN1,CAN_FIFO1,&RxMessage);	
	
	
		/* NumSet Copy */
		if((RxMessage.ExtId >= 0x1fd51630 && RxMessage.ExtId <= 0x1fd5163f)){

			for(int index1 = 0; index1 < RxMessage.DLC; index1++)
				CAN_RxMsg1[index1] = RxMessage.Data[index1];
			
				/* Msg Processing */
				VehSpd = RxMessage.Data[0];
				Gear = RxMessage.Data[1];
				TsVolt = MotorolaDataCov(RxMessage.Data[3], RxMessage.Data[2], 0, 0.1); 
				MotSpdL = MotorolaDataCov(RxMessage.Data[5], RxMessage.Data[4], -10000, 0.5); 
				MotSpdR = MotorolaDataCov(RxMessage.Data[7], RxMessage.Data[6], -10000, 0.5);
			}
		
		if(RxMessage.ExtId >= 0x1fd71630 && RxMessage.ExtId <= 0x1fd7163f){
		
			for(int index2 = 0; index2 < RxMessage.DLC; index2++)
				CAN_RxMsg2[index2] = RxMessage.Data[index2];
			
				/* Msg Processing */
				BatMaxVolt = MotorolaDataCov(RxMessage.Data[1], RxMessage.Data[0], 0, 1); 
				BatMinVolt = MotorolaDataCov(RxMessage.Data[3], RxMessage.Data[2], 0, 1); 
				PedalSta = MotorolaDataCov(RxMessage.Data[5], RxMessage.Data[4], 0, 1); 
				McuRdyL = RxMessage.Data[6];
				McuRdyR = RxMessage.Data[7];
				}
		
				
		/* Msg Processing */
				
		/* 车速 转速显示 */
		Screen_TxtNumCmd(USART2, (u8*)"VehSpd", VehSpd);
		Screen_TxtNumCmd(USART2, (u8*)"MotSpdL", MotSpdL); 
		Screen_TxtNumCmd(USART2, (u8*)"MotSpdR", MotSpdR); 
		
		/* 挡位 */
		Screen_GearMsg(USART2, (u8*)"Gear", Gear, TsVolt);
		
		/* 转速环显示 */
		Screen_StateRing(USART2, (u8*)"PedalSta", PedalSta);
		Screen_StateRing(USART2, (u8*)"MotSpdL", MotSpdL);
		Screen_StateRing(USART2, (u8*)"MotSpdR", MotSpdR);
		
		/* 高压电池 SOC */
		Screen_BattState(USART2, BatMaxVolt, BatMinVolt); 
				
		/* 电机故障信息 */
		MotWarnL_Disp = Screen_MotorWarningJudgement(USART2, "MotorL", McuRdyL);
		MotWarnR_Disp = Screen_MotorWarningJudgement(USART2, "MotorR", McuRdyR);
		

		/* ID2 Perform Persiton */
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0); 
}


/* USART DMA_IT Approach */
void USART1_IRQHandler(void)
{
		OLED_ShowString(2,1,"DU1");
	
		u8 MsgLen = 0;
		u8 tmp = 0;
	
		if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
		{
			tmp = USART1->SR;
			tmp = USART1->DR;
			tmp = tmp;
			DMA_Cmd(DMA1_Channel5, DISABLE);
			
			MsgLen = 35 - DMA_GetCurrDataCounter(DMA1_Channel5);
			//OLED_ShowNum(3, 14, MsgLen, 2);
			
			for(u8 i = 0; i < MsgLen; i++){
				//OLED_ShowChar(4, i+5, USART1_RxMsg1[i]);
			}

		}
		
		USART_ClearITPendingBit(USART1, USART_IT_IDLE); 	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
	  DMA_SetCurrDataCounter(DMA1_Channel5, 35); 
	  DMA_Cmd(DMA1_Channel5, ENABLE);        
}

void USART2_IRQHandler(void)
{
		OLED_ShowString(2,1,"D");
	
		u8 MsgLen = 0;
		u8 tmp = 0;
		char nullstr1[4] = {0x01, 0xFF, 0xFF, 0xFF};
		char nullstr2[4] = {0x1A, 0xFF, 0xFF, 0xFF};	
		char C_USART2_RxMsg1[35];
		u8 Cu8_USART2_RxMsg1[35];
		char C_TimRecRx[10];
		int C_TimRecRxNum[10];
		extern u8 TxMsg1[8];
		extern int TimLen;
	
		
		if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
		{
			tmp = USART2->SR;
			tmp = USART2->DR; 
			tmp = tmp;
			DMA_Cmd(DMA1_Channel6, DISABLE);
			
			
			MsgLen = 35 - DMA_GetCurrDataCounter(DMA1_Channel6);
			//OLED_ShowNum(3, 14, MsgLen, 2);
			
			/* String Processing */
			if((USART2_RxMsg1[0] != 0x1A) && (USART2_RxMsg1[0] != 0x01) && (USART2_RxMsg1[0] != 0xFF))
			{
				OLED_ShowString(2,2,"U2");						
				u8 ics = 0;				

				do{
					C_USART2_RxMsg1[ics] = USART2_RxMsg1[ics];
					Cu8_USART2_RxMsg1[ics] = USART2_RxMsg1[ics];
					//OLED_ShowChar(2,1+ics, USART2_RxMsg1[ics]);	
					//Usart_Send_Byte(USART1, USART2_RxMsg1[ics]);		/* USART1 通信验证 */
					
					ics++;
					
				}while((USART2_RxMsg1[ics] != 0x1A) && (USART2_RxMsg1[ics] != 0xFF) && (USART2_RxMsg1[ics] != 0x00));
				
				//Usart_Printf(USART1, Cu8_USART2_RxMsg1);
				//Usart_Send_Byte(USART1, '\n');
				//OLED_ShowNum(2,5,ics,3);
			
			
				/* Commend Recognization */
				const char Gen[] = "Gentle";
				const char Vls[] = "Violence";
				const char Mad[] = "MADNESS";
				const char Sta[] = "Standard";
				const char Lan[] = "LAUNCH";
			
				switch (C_USART2_RxMsg1[0]) {		/* 可以更新为 strncmp(char* str1, char* str2, tin n); 函数 */
				case 'G':
					if ((C_USART2_RxMsg1[1] == 'e') && (C_USART2_RxMsg1[2] == 'n')) {
						printf("%s is recognized\n", Gen);
						TxMsg1[0] = 0;
						break;
					}
					else printf("No matched\n");
					break;

				case 'V':
					if ((C_USART2_RxMsg1[1] == 'i') && (C_USART2_RxMsg1[2] == 'o')) {
						printf("%s is recognized\n", Vls);
						TxMsg1[0] = 0x2;
						break;
					}
					else printf("No matched\n");
					break;

				case 'S':
					if ((C_USART2_RxMsg1[1] == 't') && (C_USART2_RxMsg1[2] == 'a')) {
						printf("%s is recognized\n", Sta);
						TxMsg1[1] = 0x0; TxMsg1[0] = 0;
						break;
					}
					else printf("No matched\n");
					break;

				case 'M':
					if ((C_USART2_RxMsg1[1] == 'A') && (C_USART2_RxMsg1[2] == 'D')) {
						printf("%s is recognized\n", Mad);
						TxMsg1[0] = 0x4;
						break;
					}
					else printf("No matched\n");
					break;

				case 'L':
					if ((C_USART2_RxMsg1[1] == 'A') && (C_USART2_RxMsg1[2] == 'U')) {
						printf("%s is recognized\n", Lan);
						TxMsg1[1] = 0x2;
						break;
					}
					else printf("No matched\n");
					break;
			}
				
			
			/* Timer IDR Rectification */
			TimLen = 0;
			
			if (C_USART2_RxMsg1[0] == 0xAB && C_USART2_RxMsg1[1] == 0xCD){
				do{
					C_TimRecRx[TimLen] = C_USART2_RxMsg1[TimLen + 2];		/* 长度排除 IDR */
					C_TimRecRxNum[TimLen] = (int)C_USART2_RxMsg1[TimLen + 2];
					
					TimLen++;
					
				}while(C_USART2_RxMsg1[TimLen + 2] != 0x08 || C_USART2_RxMsg1[TimLen + 3] != 0x00);
				
			/* Stonge & Sort function calling */
			TimerRecord(USART2, C_TimRecRxNum, TimLen); 		
	
			}
		}
	}
		
		USART_ClearITPendingBit(USART2, USART_IT_IDLE); 	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
	  DMA_SetCurrDataCounter(DMA1_Channel6, 35); 
	  DMA_Cmd(DMA1_Channel6, ENABLE);        
}
