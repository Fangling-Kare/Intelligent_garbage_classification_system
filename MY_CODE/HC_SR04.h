#ifndef __HC_SR04_H_
#define __HC_SR04_H_

#include "ALLHeader.h"

extern float HC_distance;
extern uint8_t update_flag;

void HC_Start(void);
float HC_get_distance(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* __HC_SR04_H_ */
