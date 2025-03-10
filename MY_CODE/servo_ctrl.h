#ifndef __SERVO_CTRL_H_
#define __SERVO_CTRL_H_

#include "ALLHeader.h"

typedef struct {
    TIM_HandleTypeDef *htim;   // 定时器句柄
    uint32_t Channel;          // PWM通道
    float current_angle;       // 当前角度
    float target_angle;        // 目标角度
    float step_speed;          // 步进速度（度/更新周期）
    uint8_t is_moving;         // 运动状态标志
}ServoControl;

void servo_init(void);
void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t Channel, float angle);
void update_servo_motion(ServoControl *servo);
void set_servo_target_angle(ServoControl *servo, float target_angle, uint8_t use_smooth, float speed_deg_per_sec);


#endif /* __SERVO_CTRL_H_ */
