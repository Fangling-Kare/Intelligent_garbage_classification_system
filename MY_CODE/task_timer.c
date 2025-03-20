#include "ALLHeader.h"
#include "stm32f4xx_hal.h" 

uint8_t timer_flag = 0;
uint8_t timer_finish_flag = 1;

void task_timer_process(void)
{
    if (timer_flag)
    {
        timer_flag = 0;
        timer_finish_flag = 0;
        led_green_toggle();
        /* USER CODE BEGIN 1 */

        
        

        /* USER CODE END 1 */
        timer_finish_flag = 1;
    }
    if((timer_flag == 1 && timer_finish_flag == 1))
    {
        led_red_turn_on();
    }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1)
    {
        timer_flag = 1;
    }
}



