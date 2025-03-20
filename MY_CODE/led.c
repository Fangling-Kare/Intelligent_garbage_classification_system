#include "stm32f4xx_hal.h" 
#include "gpio.h"

// 打开LED
void led_turn_on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

// 关闭LED
void led_turn_off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

// 切换LED状态
void led_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

// 绿灯打开
void led_green_turn_on(void)
{
    led_turn_on(GPIOF,GPIO_PIN_10);
}

// 绿灯关闭
void led_green_turn_off(void)
{
    led_turn_off(GPIOF,GPIO_PIN_10);
}

// 绿灯翻转
void led_green_toggle(void)
{
    led_toggle(GPIOF,GPIO_PIN_10);
}

// 红灯打开
void led_red_turn_on(void)
{
    led_turn_on(GPIOF,GPIO_PIN_9);
}

// 红灯关闭
void led_red_turn_off(void)
{
    led_turn_off(GPIOF,GPIO_PIN_9);
}

// 红灯翻转
void led_red_toggle(void)
{
    led_toggle(GPIOF,GPIO_PIN_9);
}



