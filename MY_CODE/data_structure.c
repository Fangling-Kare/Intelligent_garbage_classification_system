#include "servo_ctrl.h"
#include "tim.h"


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

