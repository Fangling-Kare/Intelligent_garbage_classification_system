#include "ic_detection.h"


void ic_exti(void)
{
    if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_6) == GPIO_PIN_SET)
    {
        led_turn_off(GPIOF,GPIO_PIN_10);
    }
    else
    {
        led_turn_on(GPIOF,GPIO_PIN_10);
    }
}



