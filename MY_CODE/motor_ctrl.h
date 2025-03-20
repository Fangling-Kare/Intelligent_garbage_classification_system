#ifndef __MOTOR_CTRL_H_
#define __MOTOR_CTRL_H_

#include "ALLHeader.h"

void motor_Init(void);
void motor_Speed_up(uint8_t speed);
void motor_Speed_down(uint8_t speed);
void motor_Brake_up(void);
void motor_Brake_down(void);
void motor_Brake(void);
#endif /* __MOTOR_CTRL_H_ */
