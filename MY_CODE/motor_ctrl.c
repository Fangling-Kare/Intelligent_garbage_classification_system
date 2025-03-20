#include "tim.h"
#include "ALLHeader.h"
/**
  * @brief  初始化电机的定时器
  * @param  无
  * @retval 无
  */
	#define MAX_SPEED 100
	
	
void motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
}

void motor_Speed_down(uint8_t speed)
{
	if (speed > MAX_SPEED)
        speed = MAX_SPEED;
//led_toggle(GPIOF, GPIO_PIN_9);
 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void motor_Speed_up(uint8_t speed)
{
	if (speed > MAX_SPEED)
        speed = MAX_SPEED;
//led_toggle(GPIOF, GPIO_PIN_9);
 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,speed);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

void motor_Brake_down(void)
{
 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, MAX_SPEED);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void motor_Brake_up(void)
{
 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, MAX_SPEED);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}

