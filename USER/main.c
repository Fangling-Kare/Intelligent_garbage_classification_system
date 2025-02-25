/***************************************************************************************************************/ 
#include "ALLHeader.h"
/******************************************************************************************
*                        @宏定义
******************************************************************************************/
void all_init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	delay_init(168);

	led_init();

	usart_init();
	//key_init();

	__tim4_configuration(5000-1,8400-1);
	//__tim8_configuration(500-1,84-1);

	led0.toggle(&led0);
	led1.toggle(&led1);

	garbage_statistics_init(&stats);

}


int main(void)
{
	all_init();

	led1.toggle(&led1);
	uint8_t test_data[] = {0x01, 0x02, 0x03};  // 示例数据
   uint8_t data_length = sizeof(test_data);          // 数据长度

    // 发送数据包
    __usart2_dma_send_packet(test_data, data_length);
	while(1)
	{

	}
}
