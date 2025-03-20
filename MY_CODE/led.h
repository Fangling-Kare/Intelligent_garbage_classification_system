#ifndef __LED_H_
#define __LED_H_

#include "ALLHeader.h"


void led_turn_on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void led_turn_off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void led_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


void led_green_turn_on(void);
void led_green_turn_off(void);
void led_green_toggle(void);

void led_red_turn_on(void);
void led_red_turn_off(void);
void led_red_toggle(void);
#endif /* __LED_H_ */
