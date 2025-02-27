/***************************************************************************************************************/ 
#include "ALLHeader.h"
/******************************************************************************************
*                        @宏定义和数据
******************************************************************************************/
#define USART2_DMA_RX_BUFFER_MAX_LENGTH		(255)
#define USART2_DMA_TX_BUFFER_MAX_LENGTH		(255)

uint8_t USART2_DMA_RX_Buffer[USART2_DMA_RX_BUFFER_MAX_LENGTH];
uint8_t USART2_DMA_TX_Buffer[USART2_DMA_TX_BUFFER_MAX_LENGTH];

#define USART3_DMA_RX_BUFFER_MAX_LENGTH		(255)
#define USART3_DMA_TX_BUFFER_MAX_LENGTH		(255)

uint8_t USART3_DMA_RX_Buffer[USART3_DMA_RX_BUFFER_MAX_LENGTH];
uint8_t USART3_DMA_TX_Buffer[USART3_DMA_TX_BUFFER_MAX_LENGTH];


uint8_t rxd_buf[255];			//接收固定包长数
uint8_t rxd_flag =0;			//接收完成标志
uint8_t rxd_index=0;				//字节索引
uint8_t valid_data[255];  	//有效数据数组


// 定义帧头和帧尾
#define PACKET_HEADER 0x0B
#define PACKET_FOOTER 0x0E

#define MAX_PAYLOAD_LEN 64  
// 假设有效数据最大长度为64字节
#define MAX_PACKET_SIZE (1 + 1 + MAX_PAYLOAD_LEN + 1 + 1)

/******************************************************************************************
*                        @控制位
******************************************************************************************/
#define CHECK_DATA_STATE 	0 	//接收数据包：该位为0不要校验位，为1要校验位
#define USART_RX_MODE_StATE	0	//USART接收模式

/******************************************************************************************
*                        @异或计算
******************************************************************************************/
uint8_t calculate_xor_checksum(const uint8_t *data, uint8_t length) 
{
    uint8_t checksum = 0;  // 初始化异或校验值为 0

    // 遍历指定长度的数据
    for (uint8_t i = 0; i < length; i++) {
        checksum ^= data[i];  // 对每个字节进行异或运算
    }

    return checksum;  // 返回最终的异或校验值
}
/******************************************************************************************
*                        @USART2
******************************************************************************************/
void __usart2_configuration(uint32_t usart_bound)
{
	//GPIO端口设置
  	GPIO_InitTypeDef 	GPIO_InitStructure		={0};
	USART_InitTypeDef 	USART_InitStructure		={0};
	NVIC_InitTypeDef 	NVIC_InitStructure		={0};
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 							//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);							//使能USART2时钟
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 						//GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 						//GPIOA3复用为USART2
	
	//USART2端口配置
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 							//GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 									//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;									//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 											//初始化PA2，PA3

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = usart_bound;								//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
  	USART_Init(USART2, &USART_InitStructure); 										//初始化串口2
	
  	USART_Cmd(USART2, ENABLE);  													//使能串口2

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//开启接收中断
	
	//USART2 NVIC 配置
  	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;								//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化NVIC寄存器
}

void __usart2_dma_tx_configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure	= {0};
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 , ENABLE);					//DMA1时钟使能
	DMA_DeInit(DMA1_Stream6);
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);						//等待DMA可配置
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//DMA通道配置
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_DMA_TX_Buffer;	//发送缓存指针
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//DMA传输方向：内存--->外设
	DMA_InitStructure.DMA_BufferSize = USART2_DMA_TX_BUFFER_MAX_LENGTH;		//数据传输字节数量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//高优先级 DMA_Priority_High
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输

	DMA_Init(DMA1_Stream6, &DMA_InitStructure);								//初始化DMA Stream
	USART_DMACmd(USART2 , USART_DMAReq_Tx,ENABLE);  						//使能串口的DMA发送
	DMA_Cmd(DMA1_Stream6, DISABLE); 										//开启DMA传输

	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);							// 使能传输完成中断

	NVIC_InitTypeDef NVIC_InitStructure	= {0};
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;   // 中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能
    NVIC_Init(&NVIC_InitStructure);
}

void __usart2_init(uint32_t usart_bound)
{
	__usart2_configuration(usart_bound);
	__usart2_dma_tx_configuration();
}

// 发送单个字节
void __usart2_send_byte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);											//向串口2发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);					//等待发送结束
}

// 发送数组
void __usart2_send_array(uint8_t *Arr, uint8_t Length)
{
    uint8_t i;  
    for (i = 0; i < Length; ++i)
    {
        __usart2_send_byte(Arr[i]);
    }
}

void __usart2_dma_send_data(uint8_t *send_buffer , uint16_t nSendCount)
{
	if (nSendCount < USART2_DMA_TX_BUFFER_MAX_LENGTH)
	{
		memcpy(USART2_DMA_TX_Buffer , send_buffer , nSendCount);
		USART_DMACmd(USART2 , USART_DMAReq_Tx,ENABLE);  	//使能串口的DMA发送
		DMA_Cmd(DMA1_Stream6 , DISABLE);                    //关闭DMA传输
		while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);	//确保DMA可以被设置
		DMA_SetCurrDataCounter(DMA1_Stream6 , nSendCount);  //数据传输量
		DMA1_Stream6->M0AR = (uint32_t)send_buffer; 		//确保内存地址更新
		DMA_Cmd(DMA1_Stream6 , ENABLE);               		//开启DMA传输
	}
}

void __usart2_dma_send_byte(uint8_t byte)
{
    uint8_t send_buffer[1];  // 创建一个临时缓冲区来存储单个字节
    send_buffer[0] = byte;   // 将字节存储到缓冲区中

    // 调用现有的 DMA 发送函数发送单个字节
    __usart2_dma_send_data(send_buffer, 1);
}

void __usart2_dma_send_string(char *data_to_send)
{
	if (data_to_send == NULL) {
        return; // 防止空指针传入
    }
    uint16_t nSendCount = strlen(data_to_send); 			// 自动计算字符串长度
    if (nSendCount > 0) {
        __usart2_dma_send_data((uint8_t *)data_to_send, nSendCount);
    }
}



void __usart2_dma_send_packet(uint8_t *data_to_send, uint8_t data_length) 
{
    if (data_length > MAX_PAYLOAD_LEN) return;

    const uint16_t packet_size = 1 + 1 + data_length + 1 + 1;
    uint8_t packet_buffer[MAX_PACKET_SIZE];
    uint8_t *ptr = packet_buffer;
    *ptr++ = PACKET_HEADER;
    *ptr++ = data_length;
    memcpy(ptr, data_to_send, data_length);
    ptr += data_length;
    *ptr++ = calculate_xor_checksum(&packet_buffer[1], data_length + 1);
    *ptr++ = PACKET_FOOTER;

    __usart2_dma_send_data(packet_buffer, packet_size);
}
/******************************************************************************************
*                        @USART3
******************************************************************************************/
void __usart3_configuration(uint32_t usart_bound)
{
	//GPIO端口设置
  	GPIO_InitTypeDef 	GPIO_InitStructure		={0};
	USART_InitTypeDef 	USART_InitStructure		={0};
	NVIC_InitTypeDef 	NVIC_InitStructure		={0};
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 							//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);							//使能USART3时钟
 
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART2); 						//GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART2); 						//GPIOB11复用为USART3
	
	//USART3端口配置
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 						//GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 									//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;									//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); 											//初始化PB，PA3

   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = usart_bound;								//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
  	USART_Init(USART3, &USART_InitStructure); 										//初始化串口3
	
  	USART_Cmd(USART3, ENABLE);  													//使能串口3

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//开启接收中断
	
	//USART3 NVIC 配置
  	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;								//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化NVIC寄存器
}

void __usart3_dma_tx_configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure	= {0};
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 , ENABLE);					//DMA1时钟使能
	DMA_DeInit(DMA1_Stream3);
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);						//等待DMA可配置
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//DMA通道配置
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_DMA_TX_Buffer;	//发送缓存指针
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//DMA传输方向：内存--->外设
	DMA_InitStructure.DMA_BufferSize = USART2_DMA_TX_BUFFER_MAX_LENGTH;		//数据传输字节数量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//高优先级 DMA_Priority_High
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输

	DMA_Init(DMA1_Stream3, &DMA_InitStructure);								//初始化DMA Stream
	USART_DMACmd(USART3 , USART_DMAReq_Tx,ENABLE);  						//使能串口的DMA发送
	DMA_Cmd(DMA1_Stream3, DISABLE); 										//开启DMA传输

	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);							// 使能传输完成中断

	NVIC_InitTypeDef NVIC_InitStructure	= {0};
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;   // 中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能
    NVIC_Init(&NVIC_InitStructure);
}

void __usart3_init(uint32_t usart_bound)
{
	__usart3_configuration(usart_bound);
	__usart3_dma_tx_configuration();
}

// 发送单个字节
void __usart3_send_byte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);											//向串口3发送数据
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);					//等待发送结束
}

// 发送数组
void __usart3_send_array(uint8_t *Arr, uint8_t Length)
{
    uint8_t i;  
    for (i = 0; i < Length; ++i)
    {
        __usart3_send_byte(Arr[i]);
    }
}

void __usart3_dma_send_data(uint8_t *send_buffer , uint16_t nSendCount)
{
	if (nSendCount < USART2_DMA_TX_BUFFER_MAX_LENGTH)
	{
		memcpy(USART3_DMA_TX_Buffer , send_buffer , nSendCount);
		USART_DMACmd(USART3 , USART_DMAReq_Tx,ENABLE);  	//使能串口的DMA发送
		DMA_Cmd(DMA1_Stream3 , DISABLE);                    //关闭DMA传输
		while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);	//确保DMA可以被设置
		DMA_SetCurrDataCounter(DMA1_Stream3 , nSendCount);  //数据传输量
		DMA1_Stream3->M0AR = (uint32_t)send_buffer; 		//确保内存地址更新
		DMA_Cmd(DMA1_Stream3 , ENABLE);               		//开启DMA传输
	}
}

void __usart3_dma_send_string(char *data_to_send)
{
	if (data_to_send == NULL) {
        return; // 防止空指针传入
    }
    uint16_t nSendCount = strlen(data_to_send); 			// 自动计算字符串长度
    if (nSendCount > 0) {
        __usart3_dma_send_data((uint8_t *)data_to_send, nSendCount);
    }
}



/******************************************************************************************
*                        @USART
******************************************************************************************/
void __usart_init(usart_data* self)
{
	if(self->USARTx == USART2) 		__usart2_init(self->bound);
	else if(self->USARTx == USART3) __usart3_init(self->bound);
}
void usart_dma_send_data(usart_data* self,char *data_to_send)
{
	if(self->USARTx == USART2) 		__usart2_dma_send_string(data_to_send);
	else if(self->USARTx == USART3) __usart3_dma_send_string(data_to_send);
}
void usart_register_functions(usart_data* usart)
{
	usart->init = __usart_init;
	usart->tx	= usart_dma_send_data;
}
/******************************************************************************************
*                        @中断
******************************************************************************************/
void USART2_IRQHandler(void)//串口2中断服务程序
{
/*
#if USART_RX_MODE_STATE == 1
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  	{
    USART_SendData(USART2,USART_ReceiveData(USART2));
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);  
  	}  
#elif USART_RX_MODE_STATE == 0
*/
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  	{
		static uint8_t recv_state=0;
		uint16_t  recv_dat=USART_ReceiveData(USART2);
    	switch(recv_state){
		case 0:
			if(recv_dat==0x0B){
				recv_state = 1;
				rxd_index = 0;
			}
			else{
				recv_state = 0;
			}
			break;
		case 1:
			rxd_buf[rxd_index]=recv_dat;
			rxd_index++;
			if(rxd_index >= 5){ 
				recv_state =2;
			}
			break;		
		case 2:
			if(recv_dat==0x0E){
				rxd_flag =1 ;
				recv_state =0;
				uint8_t frame_len = rxd_buf[0]; // 帧长
				for (uint8_t i = 0; i < (frame_len+CHECK_DATA_STATE); i++){
                    valid_data[i] = rxd_buf[i + 1];
                }
				led1.toggle(&led1);
			}
			else {
            recv_state = 0; // 如果不是帧尾，重置状态机
			}
			break;
		}
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);  
	}  
//#endif
} 


void DMA1_Stream6_IRQHandler(void) 
{
    if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6)) 
	{
        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6); // 清除传输完成标志
    }
}

void USART3_IRQHandler(void)//串口3中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
  	{
    USART_SendData(USART3,USART_ReceiveData(USART3));
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);  
  	}  
} 
void DMA1_Stream3_IRQHandler(void) 
{
    if (DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3)) 
	{
        DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3); // 清除传输完成标志
    }
}

