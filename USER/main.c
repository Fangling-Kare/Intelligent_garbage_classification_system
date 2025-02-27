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
	__tim8_configuration(500-1,84-1);

	led0.toggle(&led0);
	led1.toggle(&led1);

	garbage_statistics_init(&stats);

	servo_init();

}


int main(void)
{
	all_init();

	led1.toggle(&led1);

	pwm_servo_set_offset(&my_servos_dowm,30);
	pwm_servo_set_position(&my_servos_dowm, 1500, 800);
	while(1)
	{
		if (rxd_flag ==1){		
			rxd_flag=0;
			__usart2_send_array(valid_data,4);
		}
		//pwm_servo_duty_compare(&my_servos_dowm);
		//delay_ms(20);
	}
}
