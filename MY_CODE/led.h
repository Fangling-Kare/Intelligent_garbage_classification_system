#ifndef __LED_H_
#define __LED_H_

#include "ALLHeader.h"


void led_turn_on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void led_turn_off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void led_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif /* __LED_H_ */
