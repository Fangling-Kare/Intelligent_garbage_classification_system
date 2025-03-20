#include "usart.h"
#include "ALLHeader.h"
 int task_1_number=1;//tn的txt该写什么，每次只加一
 int task_1_flag=1;//task1执行到哪一次（tn）,每次加4
 int task_2_flag=2;//task执行到哪一次，（tn）每次加4
 int task_3_flag=3;//task3执行到哪一次，（tn）每次加4
 int task_4_flag=4;//task4执行到哪一次，（tn）每次加4
int task_finsh_flag=0;
uint8_t __screen_task_flag=1;


//用来记录screen task进行到哪一步的标志位

#define recycle_trash_can 41

void __screen_send_data(const char *data_to_send) 
{
    //安全检查
    if (data_to_send == NULL || huart3.gState != HAL_UART_STATE_READY) {
        return;  // 可替换为错误处理
    }

    //计算实际数据长度(假设以'\a'结尾)
    uint16_t data_len = strlen(data_to_send);

    //发送数据(限制不超过最大包长度)
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)data_to_send, 
                         (data_len < MAX_PACKET_SIZE) ? data_len : MAX_PACKET_SIZE);
}
//修改txt文本
void __screen_txt(int txt_num,int data_to_send)
{
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "t%d.txt=\"%d\"\xff\xff\xff", txt_num, data_to_send);
    __screen_send_data(buffer);
}
//翻页--差个红外检验
void __screen_page(uint8_t page_id)
{	
	//大if
    if (page_id == 0)
    {
        __screen_send_data("page 0\xff\xff\xff");
    }
    else if (page_id == 1)
    {
        __screen_send_data("page 1\xff\xff\xff");
    }    
}
void __screen_success(int i)
{
	    char buffer[200];
    switch (i)
    {
        case 0:
					  snprintf(buffer, sizeof(buffer), "t%d.txt=\"分类成功\"\xff\xff\xff",i );
						__screen_send_data(buffer);
            break;
        case 1:
            	  snprintf(buffer, sizeof(buffer), "t%d.txt=\"分类成功\"\xff\xff\xff",i );
						__screen_send_data(buffer);
            break;
        default:
           	  snprintf(buffer, sizeof(buffer), "t%d.txt=\"分类成功\"\xff\xff\xff",i );
						__screen_send_data(buffer);
            break;
    }					
}


void __screen_task_fully_loaded(void)
{
if (update_flag)
	{
		 char buffer[200];
		snprintf(buffer, sizeof(buffer), "t%d.txt=\"已满载\"\xff\xff\xff",41 );
		__screen_send_data(buffer);
		
	}	


}

void __screen_task1(void)
{
if (__screen_task_flag==1)
	{
 if (task_1_flag>40)  
			{
				task_1_flag=1;
			}
		   //序号位
  	
				__screen_txt(task_1_flag,task_1_number);										
		task_1_flag=task_1_flag+4;
			task_1_number=task_1_number+1;
  			
				__screen_task_flag=2;								
			
	}
}

#if USART_RX_MODE_STATE == 0

void __screen_task2(void)
{
if(__screen_task_flag==2)
		{		
			if(task_2_flag>40)task_2_flag=2;
			char buffer[200];			
				if(  rx_buffer[0]==0xAA || rx_buffer[1]==0xAA  || rx_buffer[2]==0xAA)
				{				
					snprintf(buffer, sizeof(buffer), "t%d.txt=\"其他垃圾\"\xff\xff\xff",task_2_flag );
						__screen_send_data(buffer);	
				}				
				if(  rx_buffer[0]==0xBB || rx_buffer[1]==0xBB  || rx_buffer[2]==0xBB)
				{				
					snprintf(buffer, sizeof(buffer), "t%d.txt=\"有害垃圾\"\xff\xff\xff",task_2_flag );
						__screen_send_data(buffer);			
				}	
				if(  rx_buffer[0]==0xCC || rx_buffer[1]==0xCC  || rx_buffer[2]==0xCC)
				{				
					snprintf(buffer, sizeof(buffer), "t%d.txt=\"厨余垃圾\"\xff\xff\xff",task_2_flag);
						__screen_send_data(buffer);			
				}	
							if(  rx_buffer[0]==0xDD || rx_buffer[1]==0xDD || rx_buffer[2]==0xDD)
				{			
					snprintf(buffer, sizeof(buffer), "t%d.txt=\"可回收垃圾\"\xff\xff\xff",task_2_flag);
						__screen_send_data(buffer);			
				}	
							
					task_2_flag=task_2_flag+4;
					__screen_task_flag=3;			
		}
		
}

#elif USART_RX_MODE_STATE == 1

void __screen_task2(void)
{
if(__screen_task_flag==2)
		{		
			if(task_2_flag>40)task_2_flag=2;
			char buffer[200];			
				if(  frame_buffer[0]==0xAA || frame_buffer[1]==0xAA  || frame_buffer[2]==0xAA)
				{				
					snprintf(buffer, sizeof(buffer), "t%d.txt=\"其他垃圾\"\xff\xff\xff",task_2_flag );
						__screen_send_data(buffer);	
				}				
				if(  frame_buffer[0]==0xBB || frame_buffer[1]==0xBB  || frame_buffer[2]==0xBB)
				{				
					snprintf(buffer, sizeof(buffer), "t%d.txt=\"有害垃圾\"\xff\xff\xff",task_2_flag );
						__screen_send_data(buffer);			
				}	
				if(  frame_buffer[0]==0xCC || frame_buffer[1]==0xCC  || frame_buffer[2]==0xCC)
				{				
					snprintf(buffer, sizeof(buffer), "t%d.txt=\"厨余垃圾\"\xff\xff\xff",task_2_flag);
						__screen_send_data(buffer);			
				}	
							if(  frame_buffer[0]==0xDD || frame_buffer[1]==0xDD || frame_buffer[2]==0xDD)
				{			
					snprintf(buffer, sizeof(buffer), "t%d.txt=\"可回收垃圾\"\xff\xff\xff",task_2_flag);
						__screen_send_data(buffer);			
				}	
							
					task_2_flag=task_2_flag+4;
					__screen_task_flag=3;			
		}
		
}

#endif
void __screen_task3(void)
{
	if(__screen_task_flag==3)
	{
		if(task_3_flag>40)task_3_flag=3;
		__screen_txt(task_3_flag,1);
    task_3_flag=task_3_flag+4;
    __screen_task_flag=4;
	}	
}
void __screen_task4(void)
{	
	if(__screen_task_flag==4)
 {	
  if(task_4_flag>41)
	task_4_flag=4;
	if(task_4_flag>36)task_finsh_flag=1;
	
    __screen_success(task_4_flag);	
    task_4_flag=task_4_flag+4;
	 __screen_task_flag=1;
 } 	 
}
void __screen_task(void)
{

	if(( task_finsh_flag==0 && frame_ready == 1))
	{
		HAL_Delay(30);			 
		__screen_task1();
		 HAL_NVIC_DisableIRQ(USART2_IRQn);   
		HAL_Delay(30);
		__screen_task2();
		HAL_Delay(30);
		__screen_task3();
		HAL_Delay(30);
		__screen_task4();
		 HAL_NVIC_EnableIRQ(USART2_IRQn);   
		HAL_Delay(30);
	  //frame_ready=0;	
	}

}



















