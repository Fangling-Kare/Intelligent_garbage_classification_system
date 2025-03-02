#ifndef __DATA_STRUCTURE_H_
#define __DATA_STRUCTURE_H_

#include "ALLHeader.h"

/******************************************************************************************
*                        @舵机
******************************************************************************************/


typedef struct {
    TIM_HandleTypeDef *htim;   // 定时器句柄
    uint32_t Channel;          // PWM通道
    float current_angle;       // 当前角度
    float target_angle;        // 目标角度
    float step_speed;          // 步进速度（度/更新周期）
    uint8_t is_moving;         // 运动状态标志
} ServoControl;

// 初始化舵机控制器
ServoControl servo_down = {
    .htim = &htim4,            // 绑定TIM4
    .Channel = TIM_CHANNEL_1,  // 使用通道1
    .current_angle = 0.0f,
    .target_angle = 0.0f,
    .step_speed = 1.0f,        // 默认速度：1度/更新周期
    .is_moving = 0
};
ServoControl servo_up = {
    .htim = &htim4,            // 绑定TIM4
    .Channel = TIM_CHANNEL_2,  // 使用通道2
    .current_angle = 0.0f,
    .target_angle = 0.0f,
    .step_speed = 1.0f,        // 默认速度：1度/更新周期
    .is_moving = 0
};

#endif /* __DATA_STRUCTURE_H_ */
