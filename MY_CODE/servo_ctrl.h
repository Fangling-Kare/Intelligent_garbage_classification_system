#ifndef __SERVO_CTRL_H_
#define __SERVO_CTRL_H_

#include "ALLHeader.h"

void servo_init(void);
void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t Channel, float angle);
void update_servo_motion(ServoControl *servo);
void set_servo_target_angle(ServoControl *servo, float target_angle, uint8_t use_smooth, float speed_deg_per_sec);

#endif /* __SERVO_CTRL_H_ */
