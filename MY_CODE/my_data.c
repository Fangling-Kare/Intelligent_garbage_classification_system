/***************************************************************************************************************/ 
#include "ALLHeader.h"

/******************************************************************************************
*                        @LED
******************************************************************************************/  
led_data led0 = {
	.GPIOx = GPIOF,
	.GPIO_Pin = GPIO_Pin_9,
};// 红灯
led_data led1 = {
	.GPIOx = GPIOF,
	.GPIO_Pin = GPIO_Pin_10,
};// 绿灯

void led_init(void)
{
  led_register_functions(&led0);
	led0.init(&led0);
  led_register_functions(&led1);
	led1.init(&led1);
}

/******************************************************************************************
*                        @USART
******************************************************************************************/  
usart_data usart2 = {
	.USARTx = USART2,
	.bound	= 115200,
};

usart_data usart3 = {
	.USARTx = USART3,
	.bound	= 115200,
};
void usart_init(void)
{
	usart_register_functions(&usart2);
	usart2.init(&usart2);
	usart_register_functions(&usart3);
	usart3.init(&usart3);
}


