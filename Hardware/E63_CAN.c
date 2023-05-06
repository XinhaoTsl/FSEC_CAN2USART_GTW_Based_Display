#include <E63_CAN.h>

u8 CAN1_Configuration(void){ 
	
    GPIO_InitTypeDef        GPIO_InitStructure; 
    CAN_InitTypeDef         CAN_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);                                                       
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  
  
		GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE); 
	
		/* Configure CAN pin: RX */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);   
	
		/* Configure CAN pin: TX */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	

		//CAN Init
    CAN_InitStructure.CAN_TTCM=DISABLE;        
    CAN_InitStructure.CAN_ABOM=DISABLE;       
    CAN_InitStructure.CAN_AWUM=DISABLE;       
    CAN_InitStructure.CAN_NART=DISABLE;        
    CAN_InitStructure.CAN_RFLM=DISABLE;         
    CAN_InitStructure.CAN_TXFP=DISABLE;         
    CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;  

		//250kps ²¨ÌØÂÊ Config
    CAN_InitStructure.CAN_SJW=tsjw;         
    CAN_InitStructure.CAN_BS1=tbs1;             
    CAN_InitStructure.CAN_BS2=tbs2;           
    CAN_InitStructure.CAN_Prescaler=brp;        
    CAN_Init(CAN1, &CAN_InitStructure);      
    return 0;		
}		

void CAN1_FilterConfig(u32 CAN_FilterId, u32 CAN_Musk, u8 CAN_FilterNumber, u16 CAN_FilterFIFOAssignment){
	
		CAN_FilterInitTypeDef   CAN_FilterInitStructure;

		//ÂË²¨ Config
    CAN_FilterInitStructure.CAN_FilterNumber=CAN_FilterNumber;  //0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;   
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 
    CAN_FilterInitStructure.CAN_FilterIdHigh= ((CAN_FilterId<<3) >>16) &0xffff;     //((0x Msg_ID <<3) >>16) &0xffff
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_FilterId<<3) &0xffff;					    //(0x Msg_ID <<3) &0xffff
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= ((CAN_Musk<<3) >>16) &0xffff;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow= ((CAN_Musk<<3) &0xffff);
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFOAssignment;     //CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
	
		return;
}

void CAN1_ITConfig(void){
	
		NVIC_InitTypeDef        NVIC_InitStructure;	 
	
    CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);        // Enable Rx IT & Error IT(off line)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

		return;
}


u8 CAN_Send_Msg(u32 MsgId, u8* msg,u8 len){   
    u8 mbox;
    u16 i=0;
    CanTxMsg TxMessage;
	
    //TxMessage.StdId=0x12;           
    TxMessage.ExtId = MsgId;           
    TxMessage.IDE = CAN_Id_Extended;  
    TxMessage.RTR = CAN_RTR_Data;     
    TxMessage.DLC = len;   
	
    for(i=0;i<len;i++)
    TxMessage.Data[i] = msg[i];      
    mbox= CAN_Transmit(CAN1,&TxMessage);   
    i=0; 
    while((CAN_TransmitStatus(CAN1,mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;
    if(i>=0XFFF)return 1;
    return mbox;    
}


u8 CAN_Receive_Msg(u8 *ptr){    

    CanRxMsg RxMessage;	
	
    u8 i;
		u8 dlc;
	
    RxMessage.StdId = 0x00;
    RxMessage.ExtId = 0x00;
    RxMessage.IDE = 0;
    RxMessage.RTR = 0;
    RxMessage.DLC = 0;
    RxMessage.FMI = 0;	
	
    //if(CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;
    CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
    
		dlc = RxMessage.DLC;
	
		for(i = 0; i < dlc; i++) 
    ptr[i]=RxMessage.Data[i];  
	
    return dlc;  
}


int MotorolaDataCov(int num_2B, int num_1B, int offset, double gain)
{
		int index1 = 0;
		int index2 = 0;
		int NumCov1[8] = { 0 };
		int NumCov2[8] = { 0 };

		if (num_2B != 0)
			while (num_2B > 0) {
				NumCov1[index1] = num_2B % 2;
				index1 += 1;
				num_2B /= 2;
			}
		else;

		if (num_1B != 0)
			while (num_1B > 0) {
				NumCov2[index2] = num_1B % 2;
				index2 += 1;
				num_1B /= 2;
			}
		else;

		int BinNumComb[16] = { 0 };

		for (int i = 1; 8 - i >= 0; i++)
			BinNumComb[i - 1] = NumCov1[8 - i];
		for (int ii = 1; 8 - ii >= 0; ii++)
			BinNumComb[7 + ii] = NumCov2[8 - ii];

		int CombNum = 0;
		int ExpIndex = 0;

		for (int xindex = 1; 16 - xindex >= 0; xindex++) {
			if (xindex - 1 == 0) ExpIndex = 1;
			else ExpIndex *= 2;
			CombNum += (BinNumComb[16 - xindex]) * ExpIndex;
		}

		return (gain * (double)CombNum) + offset;

}
