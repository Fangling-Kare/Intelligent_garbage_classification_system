#include "tim.h"

/**
  * @brief  初始化电机的定时器
  * @param  无
  * @retval 无
  */
void motor_init(void)
{
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
}

