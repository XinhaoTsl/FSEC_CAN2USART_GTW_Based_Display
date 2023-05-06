#include <E63_USART1.h>

void Usart_InitCongif(USART_TypeDef* USARTx, USARTx_InitDef * USARTx_InitMsg, u32 USARTx_BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	
	if (USARTx == USART1)
		RCC_APB2PeriphClockCmd(USARTx_InitMsg->GPIOx_RCC | USARTx_InitMsg->USARTx_RCC | RCC_APB2Periph_AFIO, ENABLE); 	
	else{
	RCC_APB2PeriphClockCmd(USARTx_InitMsg->GPIOx_RCC | RCC_APB2Periph_AFIO, ENABLE); 	
	RCC_APB1PeriphClockCmd(USARTx_InitMsg->USARTx_RCC, ENABLE);	
	}

	/* TX */
	GPIO_InitStruct.GPIO_Pin = USARTx_InitMsg->USARTx_PinTx;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USARTx_InitMsg->GPIOx, &GPIO_InitStruct);
	
	/* RX */
	GPIO_InitStruct.GPIO_Pin = USARTx_InitMsg->USARTx_PinRx;		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;       // GPIO_Mode_IN_FLOATING;
	GPIO_Init(USARTx_InitMsg->GPIOx, &GPIO_InitStruct);

	
	/* USART1 Config*/
	USART_DeInit(USARTx);
	USART_StructInit(&USART_InitStruct);
	
	USART_InitStruct.USART_BaudRate = USARTx_BaudRate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USARTx, &USART_InitStruct);
	USART_Cmd(USARTx, ENABLE);
}

void Usart1_IT_Config(void)
{
		NVIC_InitTypeDef  NVIC_InitStruct;	 
	
		//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //接收中断
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); 	//空闲中断
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  //DMA 使能
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn ;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
}

void USART1_DAM_Config(u32 paddr, u32 maddr, u16 len)
{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
		DMA_DeInit(DMA1_Channel5);
	
		DMA_InitTypeDef DMA_InitStruct;
		DMA_InitStruct.DMA_PeripheralBaseAddr = paddr;
		DMA_InitStruct.DMA_MemoryBaseAddr = maddr;
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStruct.DMA_BufferSize = len;
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
		DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

		DMA_Init(DMA1_Channel5, &DMA_InitStruct);
		DMA_Cmd(DMA1_Channel5, ENABLE);
}

void Usart2_IT_Config(void)
{
		NVIC_InitTypeDef  NVIC_InitStruct;	 
	
		//USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	  //接收中断
		//USART_ITConfig(USART2, USART_IT_TXE, ENABLE); 	//发送中断
		USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); 	//空闲中断
		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);  //DMA 使能
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn ;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
}

void USART2_DAM_Config(u32 paddr, u32 maddr, u16 len)
{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
		DMA_DeInit(DMA1_Channel6);
	
		DMA_InitTypeDef DMA_InitStruct;
		DMA_InitStruct.DMA_PeripheralBaseAddr = paddr;
		DMA_InitStruct.DMA_MemoryBaseAddr = maddr;
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStruct.DMA_BufferSize = len;
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
		DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel6, &DMA_InitStruct);
		DMA_Cmd(DMA1_Channel6, ENABLE);
}

void NVIC_Config(void)
{
	/* Receive Data register not empty interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
	
   NVIC_InitTypeDef NVIC_InitStructure; 
    
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    
   NVIC_Init(&NVIC_InitStructure);
}

	/* printf 补丁函数 */
int fputc(int ch, FILE *f)
{
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
    {
    }
    USART_SendData(USART1, (uint8_t) ch);
    return ch;
}

void Usart_Printf(USART_TypeDef* USARTx, u8 *str){
		
		u8 data=0;
		do{
	
		  USART_SendData(USARTx ,str[data]);
		  while(USART_GetFlagStatus(USARTx ,USART_FLAG_TXE)==RESET);
			data++;
		}
		while(str[data] != '\0');               
		
	}

void Usart_Send_Byte(USART_TypeDef* USARTx, char Usart_Byte)
{
		USARTx->DR = (Usart_Byte & (uint16_t)0x01FF);
		while(USART_GetFlagStatus(USARTx ,USART_FLAG_TXE)==RESET);
}


void Usart_Send_Str(USART_TypeDef* USARTx, u8 *str)
{
	while(*str!='\0')
	{
		Usart_Send_Byte(USARTx, *str++);
	}
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	
}

void Usart_Send_Int2Str(USART_TypeDef* USARTx, int num)
{
		int i = 0, j = 0;
		u8 temp[10], str[10];

		if(num == 0) {
			temp[0] = '0';
			i++;
		}
		else
			while (num)
			{
				temp[i++] = num % 10 + '0'; 
				num /= 10;
			}
			
		temp[i] = '\0';
		i = i - 1;
		while (i >= 0)
			str[j++] = temp[i--];
		str[j] = '\0';
		
		u8 *StrPtr;
		StrPtr = str;
		
		Usart_Send_Str(USARTx, StrPtr);
}


char* Int2Str(USART_TypeDef* USARTx, int num)
{
		int i = 0, j = 0;
		u8 temp[10], str[10];

		if(num == 0) {
			temp[0] = '0';
			i++;
		}
		else
			while (num)
			{
				temp[i++] = num % 10 + '0'; 
				num /= 10;
			}
			
		temp[i] = '\0';
		i = i - 1;
		while (i >= 0)
			str[j++] = temp[i--];
		str[j] = '\0';
		
		char *StrPtr;
		StrPtr = str;
		
		return StrPtr;
}


void Usart_Cmd_END(USART_TypeDef* USARTx)
{
		Usart_Send_Byte(USARTx, 0xFF);
		Usart_Send_Byte(USARTx, 0xFF);
		Usart_Send_Byte(USARTx, 0xFF);
}


void Screen_NumCmd(USART_TypeDef* USARTx, u8 *ScreenNumCmd, int num)
{
		Usart_Printf(USARTx, ScreenNumCmd);
		Usart_Send_Byte(USARTx, '=');
		Usart_Send_Int2Str(USARTx, num);
		Usart_Cmd_END(USARTx);
}


void Screen_u8NumCmd(USART_TypeDef* USARTx, u8 *ScreenNumCmd, u8 num)
{
		Usart_Printf(USARTx, ScreenNumCmd);
		Usart_Send_Byte(USARTx, '=');
		Usart_Send_Byte(USARTx, num);
		Usart_Cmd_END(USARTx);
}

void Screen_TxtNumCmd(USART_TypeDef* USARTx, u8 *ScreenNumCmd, int num)
{
	
		/* 车速示数 */
		if(strcmp((char*)ScreenNumCmd, "VehSpd") == 0){
			
			Usart_Printf(USARTx, (u8*)"t5.txt=");
			
			if(num > 0 && num < 10){
				Usart_Printf(USARTx, (u8*)"\"0");	
				Usart_Send_Int2Str(USARTx, num);
				Usart_Printf(USARTx, (u8*)"\"");
				Usart_Cmd_END(USARTx);	
				
				Usart_Printf(USARTx, (u8*)"t6.txt=\"0\""); Usart_Cmd_END(USARTx);
			}

			else if(num >= 10 && num < 100){
				Usart_Printf(USARTx, (u8*)"\"");			
				Usart_Send_Int2Str(USARTx, num);
				Usart_Printf(USARTx, (u8*)"\"");
				Usart_Cmd_END(USARTx);				
				
				Usart_Printf(USARTx, (u8*)"t6.txt=\"0\""); Usart_Cmd_END(USARTx);				
			}

			else if(num >= 100 && num < 200){
				Usart_Printf(USARTx, (u8*)"\"");
				Usart_Send_Int2Str(USARTx, (num/10) % 10);
				Usart_Send_Int2Str(USARTx, num % 10);
				Usart_Printf(USARTx, (u8*)"\"");
				Usart_Cmd_END(USARTx);				
				
				Usart_Printf(USARTx, (u8*)"t6.txt=\"1\""); Usart_Cmd_END(USARTx);		
			}
			
			else if(num == 0){
				Usart_Printf(USARTx, (u8*)"\"00\"");	
				Usart_Cmd_END(USARTx);
			}
			
			else ; //Error Data
			
			if(num == 0){
				Usart_Printf(USARTx, (u8*)"z0.val=0");
				Usart_Cmd_END(USARTx);
			} 
			else{
				Usart_Printf(USARTx, (u8*)"z0.val=");
				if(num <= 100) Usart_Send_Int2Str(USARTx, (int)((double)num * 2.5));
				else Usart_Send_Int2Str(USARTx, 250);
				Usart_Cmd_END(USARTx);			
			}

		}
		

		/* 电机转速示数 */
		if(strcmp((char*)ScreenNumCmd, "MotSpdL") == 0 || strcmp((char*)ScreenNumCmd, "MotSpdR") == 0){
			
			if(strcmp((char*)ScreenNumCmd, "MotSpdL") == 0)
				Usart_Printf(USARTx, (u8*)"t0.txt=");
			if(strcmp((char*)ScreenNumCmd, "MotSpdR") == 0)
				Usart_Printf(USARTx, (u8*)"t1.txt=");			
			
			if(num >= 0 && num < 10)
				Usart_Printf(USARTx, (u8*)"\"000");
			else if(num >= 10 && num < 100)
				Usart_Printf(USARTx, (u8*)"\"00");
			else if(num >= 100 && num < 1000)
				Usart_Printf(USARTx, (u8*)"\"0");
			else if(num >= 1000 && num < 5000)
				Usart_Printf(USARTx, (u8*)"\"");
			else ; // Error Data
				
			Usart_Send_Int2Str(USARTx, num);
			Usart_Printf(USARTx, (u8*)"\"");	
			Usart_Cmd_END(USARTx);			
		}
}

void Screen_StateRing(USART_TypeDef* USARTx, u8 *ScreenNumCmd, int num)
{
		/* 左电机转速环 */
		if(strcmp((char*)ScreenNumCmd, "MotSpdL") == 0){
			if(num >= 0 && num < 10){
				Usart_Printf(USARTx, (u8*)"p10.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p11.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p12.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p13.aph=0"); Usart_Cmd_END(USARTx);
			}
			else if(num >= 10 && num < 600){
				Usart_Printf(USARTx, (u8*)"p10.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p11.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p12.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p13.aph=127"); Usart_Cmd_END(USARTx);			
			}
			else if(num >= 600 && num < 2200){
				Usart_Printf(USARTx, (u8*)"p10.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p11.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p12.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p13.aph=127"); Usart_Cmd_END(USARTx);			
			}
			else if(num >= 2200 && num < 3500){
				Usart_Printf(USARTx, (u8*)"p10.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p11.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p12.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p13.aph=127"); Usart_Cmd_END(USARTx);				
			}
			else if(num >= 3500 && num < 5000){
				Usart_Printf(USARTx, (u8*)"p10.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p11.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p12.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p13.aph=127"); Usart_Cmd_END(USARTx);	
			}
			else; //Error Data
		}

		/* 右电机转速环 */
		if(strcmp((char*)ScreenNumCmd, "MotSpdR") == 0){
			if(num >= 0 && num < 10){
				Usart_Printf(USARTx, (u8*)"p17.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p16.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p15.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p14.aph=0"); Usart_Cmd_END(USARTx);
			}
			else if(num >= 10 && num < 600){
				Usart_Printf(USARTx, (u8*)"p17.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p16.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p15.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p14.aph=127"); Usart_Cmd_END(USARTx);		
			}
			else if(num >= 600 && num < 2200){
				Usart_Printf(USARTx, (u8*)"p17.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p16.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p15.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p14.aph=127"); Usart_Cmd_END(USARTx);		
			}
			else if(num >= 2200 && num < 3500){
				Usart_Printf(USARTx, (u8*)"p17.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p16.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p15.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p14.aph=127"); Usart_Cmd_END(USARTx);				
			}
			else if(num >= 3500 && num < 5000){
				Usart_Printf(USARTx, (u8*)"p17.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p16.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p15.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p14.aph=127"); Usart_Cmd_END(USARTx);
			}
			else; //Error Data
		}
		
		/* 踏板百分比 */
		if(strcmp((char*)ScreenNumCmd, "PedalSta") == 0){
			if(num >= 0 && num < 20){
				Usart_Printf(USARTx, (u8*)"p6.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p7.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p8.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p9.aph=0"); Usart_Cmd_END(USARTx);				
			}
			else if(num >= 20 && num < 200){
				Usart_Printf(USARTx, (u8*)"p6.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p7.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p8.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p9.aph=127"); Usart_Cmd_END(USARTx);	
			}
			else if(num >= 200 && num < 500){
				Usart_Printf(USARTx, (u8*)"p6.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p7.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p8.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p9.aph=127"); Usart_Cmd_END(USARTx);	
			}
			else if(num >= 500 && num < 700){
				Usart_Printf(USARTx, (u8*)"p6.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p7.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p8.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p9.aph=127"); Usart_Cmd_END(USARTx);			
			}
			else if(num >= 700 && num < 1000){
				Usart_Printf(USARTx, (u8*)"p6.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p7.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p8.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p9.aph=127"); Usart_Cmd_END(USARTx);						
			}
			else; //Error Data
		}
}
		
void Screen_BattState(USART_TypeDef* USARTx, int UpperVolt, int LowerVolt)
{
		/* 电池满电状态 */
		if(UpperVolt >= 4100){
			Usart_Printf(USARTx, (u8*)"t2.txt=\"FULL ");
			Usart_Send_Int2Str(USARTx, UpperVolt);
			Usart_Send_Byte(USARTx, '/');
			Usart_Send_Int2Str(USARTx, LowerVolt);
			Usart_Printf(USARTx, (u8*)"\"");
			Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"t2.pco=65535"); Usart_Cmd_END(USARTx);	
			Usart_Printf(USARTx, (u8*)"t11.aph=0"); Usart_Cmd_END(USARTx);
			
			Usart_Printf(USARTx, (u8*)"p1.aph=127"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p2.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p3.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p4.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p5.aph=0"); Usart_Cmd_END(USARTx);
		}
		
		/* 电池次满电状态 */
		else if(LowerVolt <= 4000 && LowerVolt >= 3900 && UpperVolt < 4100){
			Usart_Printf(USARTx, (u8*)"t2.txt=\"SubF ");
			Usart_Send_Int2Str(USARTx, UpperVolt);
			Usart_Send_Byte(USARTx, '/');
			Usart_Send_Int2Str(USARTx, LowerVolt);
			Usart_Printf(USARTx, (u8*)"\"");
			Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"t2.pco=65535");	Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"t11.aph=0"); Usart_Cmd_END(USARTx);			
			
			Usart_Printf(USARTx, (u8*)"p1.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p2.aph=127"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p3.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p4.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p5.aph=0"); Usart_Cmd_END(USARTx);		
		}
		
		/* 电池半满电状态 */
		else if(LowerVolt >= 3780 && LowerVolt < 3900){
			Usart_Printf(USARTx, (u8*)"t2.txt=\"Half ");
			Usart_Send_Int2Str(USARTx, UpperVolt);
			Usart_Send_Byte(USARTx, '/');
			Usart_Send_Int2Str(USARTx, LowerVolt);
			Usart_Printf(USARTx, (u8*)"\"");
			Usart_Cmd_END(USARTx);	
			Usart_Printf(USARTx, (u8*)"t2.pco=65535");	Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"t11.aph=0"); Usart_Cmd_END(USARTx);			
			
			Usart_Printf(USARTx, (u8*)"p1.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p2.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p3.aph=127"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p4.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p5.aph=0"); Usart_Cmd_END(USARTx);
		}
		
		/* 电池次亏电状态 */
		else if(LowerVolt >= 3720 && LowerVolt < 3780){
			Usart_Printf(USARTx, (u8*)"t2.txt=\"SubL ");
			Usart_Send_Int2Str(USARTx, UpperVolt);
			Usart_Send_Byte(USARTx, '/');
			Usart_Send_Int2Str(USARTx, LowerVolt);
			Usart_Printf(USARTx, (u8*)"\"");
			Usart_Cmd_END(USARTx);		
			Usart_Printf(USARTx, (u8*)"t2.pco=65535"); Usart_Cmd_END(USARTx);	
			Usart_Printf(USARTx, (u8*)"t11.aph=0"); Usart_Cmd_END(USARTx);
			
			Usart_Printf(USARTx, (u8*)"p1.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p2.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p3.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p4.aph=127"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p5.aph=0"); Usart_Cmd_END(USARTx);			
		}
		
		/* 电池亏电状态 */
		else if(LowerVolt < 3720 && LowerVolt > 3000){
			Usart_Printf(USARTx, (u8*)"t2.txt=\"LowBattPwr ");
			Usart_Send_Int2Str(USARTx, LowerVolt);
			Usart_Printf(USARTx, (u8*)"\"");
			Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"t2.pco=65504"); Usart_Cmd_END(USARTx);	
			Usart_Printf(USARTx, (u8*)"t11.aph=0"); Usart_Cmd_END(USARTx);
			
			Usart_Printf(USARTx, (u8*)"p1.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p2.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p3.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p4.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p5.aph=127"); Usart_Cmd_END(USARTx);	
			
		}
		
		/* 尚未接收到有效信息 */
		else{
			Usart_Printf(USARTx, (u8*)"t2.txt=\"Invalid Msg\""); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"t11.aph=127"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"t2.pco=65504"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p1.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p2.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p3.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p4.aph=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"p5.aph=0"); Usart_Cmd_END(USARTx);
		}
}

void Screen_GearMsg(USART_TypeDef* USARTx, u8 *ScreenNumCmd, int GearNum, int TsVolt)
{
		if(strcmp((char*)ScreenNumCmd, "Gear") == 0){
			if(GearNum == 0 && TsVolt < 300){
				Usart_Printf(USARTx, (u8*)"t7.txt=\"Park\""); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"z0.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p18.aph=0"); Usart_Cmd_END(USARTx);
			}
			else if(GearNum == 0 && TsVolt > 300){
				Usart_Printf(USARTx, (u8*)"t7.txt=\"Steady\""); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"z0.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p18.aph=0"); Usart_Cmd_END(USARTx);
			}
			else if(GearNum == 2 && TsVolt > 300){
				Usart_Printf(USARTx, (u8*)"t7.txt=\"Ready\""); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"z0.aph=127"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p18.aph=127"); Usart_Cmd_END(USARTx);
			}
			else{
				Usart_Printf(USARTx, (u8*)"t7.txt=\"Park\""); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"z0.aph=0"); Usart_Cmd_END(USARTx);
				Usart_Printf(USARTx, (u8*)"p18.aph=0"); Usart_Cmd_END(USARTx);			
			}
		}
}

int Screen_MotorWarningJudgement(USART_TypeDef* USARTx, char* WhichMotor, int StaNum)
{
		if(strcmp(WhichMotor, "MotorL") == 0){
			if(StaNum == 0 || StaNum == 3) return 0;     // 无故障
			else{																				 // 故障
				Usart_Printf(USARTx, (u8*)"t9.txt=\"");
				
				int SetNumIndex = 0;
				int NumCov1[8] = {0,0,0,0,0,0,0,0};
				
				while (StaNum > 0) {
					NumCov1[SetNumIndex] = StaNum % 2;
					SetNumIndex += 1;
					StaNum /= 2;}
				
				if(NumCov1[2])
					Usart_Printf(USARTx, (u8*)"RotEncoder "); 
				if(NumCov1[3])
					Usart_Printf(USARTx, (u8*)"OverCurrent "); 
				if(NumCov1[4])
					Usart_Printf(USARTx, (u8*)"OverVoltage "); 
				if(NumCov1[5])
					Usart_Printf(USARTx, (u8*)"Ctrl_elect "); 
				if(NumCov1[6])
					Usart_Printf(USARTx, (u8*)"UnderVoltage "); 
				if(NumCov1[7])
					Usart_Printf(USARTx, (u8*)"ExPwrRange "); 
				
				Usart_Printf(USARTx, (u8*)"ERROR\""); 
				Usart_Cmd_END(USARTx);
				return 1;
			}
		}
		
		if(strcmp(WhichMotor, "MotorR") == 0){
			if(StaNum == 0 || StaNum == 3) return 0;     // 无故障
			else{																				 // 故障
				Usart_Printf(USARTx, (u8*)"t8.txt=\"");
				
				int SetNumIndex = 0;
				int NumCov2[8] = {0,0,0,0,0,0,0,0};
				
				while (StaNum > 0) {
					NumCov2[SetNumIndex] = StaNum % 2;
					SetNumIndex += 1;
					StaNum /= 2;}
				
				if(NumCov2[2])
					Usart_Printf(USARTx, (u8*)"RotEncoder "); 
				if(NumCov2[3])
					Usart_Printf(USARTx, (u8*)"OverCurrent "); 
				if(NumCov2[4])
					Usart_Printf(USARTx, (u8*)"OverVoltage "); 
				if(NumCov2[5])
					Usart_Printf(USARTx, (u8*)"Ctrl_elect "); 
				if(NumCov2[6])
					Usart_Printf(USARTx, (u8*)"UnderVoltage "); 
				if(NumCov2[7])
					Usart_Printf(USARTx, (u8*)"ExPwrRange "); 
				
				Usart_Printf(USARTx, (u8*)"ERROR\""); 
				Usart_Cmd_END(USARTx);
				return 1;
			}
		}
}


int Screen_TimerRecCmd(USART_TypeDef* USARTx, char* CmdMsg)
{
		if(strcmp(CmdMsg, "Init") == 0)
			TimerDisp(USARTx);
		
		if(strcmp(CmdMsg, "Start") == 0){
			Usart_Printf(USARTx, (u8*)"tm0.en=1"); Usart_Cmd_END(USARTx);
		}
		
		if(strcmp(CmdMsg, "Rec") == 0){
			Usart_Printf(USARTx, (u8*)"sys5=1033"); Usart_Cmd_END(USARTx);
		}
		
		if(strcmp(CmdMsg, "Pause") == 0){
			Usart_Printf(USARTx, (u8*)"tm0.en=0"); Usart_Cmd_END(USARTx);
		}
		
		if(strcmp(CmdMsg, "Reset") == 0){
			Usart_Printf(USARTx, (u8*)"tm0.en=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"n0.val=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"n1.val=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"n2.val=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"sys3=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"sys4=0"); Usart_Cmd_END(USARTx);
			Usart_Printf(USARTx, (u8*)"sys5=0"); Usart_Cmd_END(USARTx);
			
			//Usart_Printf(USARTx, (u8*)"sys5=717"); Usart_Cmd_END(USARTx);    /* 指令 BUG */
		}
		
		if(strcmp(CmdMsg, "Exit") == 0){
			TimerHide(USARTx);
			Usart_Printf(USARTx, (u8*)"t14.aph=0"); Usart_Cmd_END(USARTx);
		}

}

int TimerDisp(USART_TypeDef* USARTx)
{
	Usart_Printf(USARTx, (u8*)"n0.aph=127"); Usart_Cmd_END(USARTx);
	Usart_Printf(USARTx, (u8*)"n1.aph=127"); Usart_Cmd_END(USARTx);
	Usart_Printf(USARTx, (u8*)"n2.aph=127"); Usart_Cmd_END(USARTx);
	Usart_Printf(USARTx, (u8*)"t13.aph=127"); Usart_Cmd_END(USARTx);
}

int TimerHide(USART_TypeDef* USARTx)
{
	Usart_Printf(USARTx, (u8*)"n0.aph=0"); Usart_Cmd_END(USARTx);
	Usart_Printf(USARTx, (u8*)"n1.aph=0"); Usart_Cmd_END(USARTx);
	Usart_Printf(USARTx, (u8*)"n2.aph=0"); Usart_Cmd_END(USARTx);
	Usart_Printf(USARTx, (u8*)"t13.aph=0"); Usart_Cmd_END(USARTx);
}


int TimerSort(int countss, int countpr, long TimRec)
{
	long temp;
	static long ptr[TimRecLen];

	if (countss == 3 && countpr == 0 && TimRec != 0)
		*ptr = TimRec;
	
	else if (TimRec < 1000)   /* 排除无效数据 */
		;
	
	else if (countpr == 3 && countss == 1 && TimRec == 0)
		memset(ptr, 0, sizeof(ptr));    		 /* 清空数组 */
	
	else if (countss > 3 && countss < TimRecLen && countpr == 0 && TimRec != 0){
		*(ptr + (countss - 3)) = TimRec;
		for(int findex = 0; findex < (countss - 3); findex++){
			for(int lindex = findex + 1; lindex < (countss - 2); lindex++){
				if(ptr[lindex] < ptr[findex]){
					temp = ptr[findex];
					ptr[findex] = ptr[lindex];
					ptr[lindex] = temp;
				}
			}
		}
	}
	
	int index = 0;
		
	while(ptr[index] == 0)
			index++;
	
	return ptr[index];
}


int TimerRecord(USART_TypeDef* USARTx, int* TimRec, int BitLen)
{
	long RealTimRec;
	long OptimalRtr;
	extern int TimCount12;
	extern int TimCount13;
	static int Trf_CallingTimes = 0; if (TimCount12 == 3) Trf_CallingTimes = 0;
	
	if(BitLen == 1)
		RealTimRec = *TimRec;
	
	else if (BitLen == 2)
		RealTimRec = ((*TimRec) & 0xff) + (((*(TimRec + 1)) & 0xffff) << 8);
	
	else if (BitLen == 3)
		RealTimRec = ((*TimRec) & 0xff) + (((*(TimRec + 1)) & 0xffff) << 8) + 
			(((*(TimRec + 2)) & 0xffffff) << 16);
	
	else if (BitLen == 4)
		RealTimRec = ((*TimRec) & 0xff) + (((*(TimRec + 1)) & 0xffff) << 8) + 
			(((*(TimRec + 2)) & 0xffffff) << 16) + (((*(TimRec + 3)) & 0xffffffff) << 24);
	
	/* Timer sort */
	OptimalRtr = TimerSort(TimCount12, TimCount13, RealTimRec);
	
	int TimMs = OptimalRtr % 1000; char C_TimMs[10]; strcpy(C_TimMs, Int2Str(USARTx, TimMs));
	int TimMin = OptimalRtr/1000/60; char C_TimMin[10]; strcpy(C_TimMin, Int2Str(USARTx, TimMin));
	int TimSec = OptimalRtr/1000 - 60 * TimMin; char C_TimSec[10]; strcpy(C_TimSec, Int2Str(USARTx, TimSec));
	
	/* Display config */
	Usart_Printf(USARTx, (u8*)"t14.txt=\"");
	Usart_Send_Int2Str(USARTx, TimMin); Usart_Send_Byte(USARTx, ':');
	
	if(TimSec < 10) Usart_Send_Byte(USARTx, '0');
	Usart_Send_Int2Str(USARTx, TimSec); Usart_Send_Byte(USARTx, ':');
	
	Usart_Send_Int2Str(USARTx, TimMs);
	
	Usart_Printf(USARTx, (u8*)"\"");
	Usart_Cmd_END(USARTx);
	
	/* USART1 Varification */
	Usart_Printf(USART1, (u8*)"t14.txt=\"");
	Usart_Send_Int2Str(USART1, TimMin); 
	Usart_Send_Int2Str(USART1, TimSec); 
	Usart_Send_Int2Str(USART1, TimMs);
	Usart_Printf(USART1, (u8*)"\"");
	Usart_Cmd_END(USART1);
	
	
	Usart_Printf(USARTx, (u8*)"t14.aph=127"); Usart_Cmd_END(USARTx);
	
	Trf_CallingTimes++;
	return Trf_CallingTimes;
}


