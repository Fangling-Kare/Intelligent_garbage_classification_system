#include "tim.h"
#include "servo_ctrl.h"

/**
  * @brief  初始化舵机的定时器
  * @param  无
  * @retval 无
  */
void servo_init(void)
{
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
}

/**
  * @brief  设置舵机角度
  * @param  htim: 定时器句柄（如&TIM4_Handle）
  * @param  Channel: PWM通道（如TIM_CHANNEL_1）
  * @param  angle: 目标角度（-135.0~+135.0度）
  * @retval 无
  */
 void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t Channel, float angle)
 {
     // 角度限幅（-135°~+135°）
     if(angle < -135.0f) angle = -135.0f;
     if(angle > 135.0f) angle = 135.0f;
     
     // 将角度线性映射到脉宽（500~2500μs）
     float pulse_width_us = 1500.0f + (angle / 135.0f) * 1000.0f;
     
     // 根据定时器配置计算CCR值
     // 定时器时钟 = 84MHz/(84) = 1MHz → 1μs/tick
     uint16_t ccr_value = (uint16_t)(pulse_width_us);
     
     // 设置捕获比较寄存器
     __HAL_TIM_SET_COMPARE(htim, Channel, ccr_value);
 }

/**
  * @brief  非阻塞平滑移动更新函数，需周期性调用（如主循环或定时器中断）
  * @param  servo: 舵机控制对象指针
  * @retval 无
  */
void update_servo_motion(ServoControl *servo) {
    if (!servo->is_moving) return;

    // 计算剩余角度差
    float angle_diff = servo->target_angle - servo->current_angle;

    if (fabs(angle_diff) > servo->step_speed) {
        // 按步进速度更新角度
        servo->current_angle += (angle_diff > 0) ? servo->step_speed : -servo->step_speed;
    } else {
        // 到达目标角度
        servo->current_angle = servo->target_angle;
        servo->is_moving = 0;
    }

    // 更新实际PWM输出
    set_servo_angle(servo->htim, servo->Channel, servo->current_angle);
}

/**
  * @brief  设置舵机目标角度（支持直接设置或平滑移动）
  * @param  servo: 舵机控制对象指针
  * @param  target_angle: 目标角度（-135.0~+135.0度）
  * @param  use_smooth: 是否启用平滑移动（1=启用，0=立即设置）
  * @param  speed_deg_per_sec: 平滑移动速度（度/秒，仅use_smooth=1时有效）
  * @retval 无
  */
 void set_servo_target_angle(ServoControl *servo, float target_angle, uint8_t use_smooth, float speed_deg_per_sec) {
    // 角度限幅
    target_angle = (target_angle < -135.0f) ? -135.0f : (target_angle > 135.0f) ? 135.0f : target_angle;

    if (!use_smooth) {
        // 直接设置角度
        servo->current_angle = target_angle;
        servo->target_angle = target_angle;
        set_servo_angle(servo->htim, servo->Channel, target_angle);
        servo->is_moving = 0;
    } else {
        // 配置平滑移动参数
        servo->target_angle = target_angle;
        servo->step_speed = speed_deg_per_sec * 0.02f;  // 假设每20ms更新一次（需与实际调用周期匹配）
        servo->is_moving = 1;
    }
}
/******************************************************************************************
*                        @如何使用
******************************************************************************************/
/*
1.初始化舵机
// 启动PWM
HAL_TIM_PWM_Start(my_servo.htim, my_servo.Channel);

// 初始位置设为0度
SetServoTargetAngle(&my_servo, 0.0f, 0, 0);

2.主循环中调用更新函数
while (1) {
    // 非阻塞更新舵机位置
    UpdateServoMotion(&my_servo);

    // 其他任务...
    HAL_Delay(20);  // 控制更新周期为20ms（与step_speed计算匹配）
}

3.触发角度变化
// 立即转到90度（无平滑）
SetServoTargetAngle(&my_servo, 90.0f, 0, 0);

// 平滑移动到-45度，速度30度/秒
SetServoTargetAngle(&my_servo, -45.0f, 1, 30.0f);

4.完整调用流程图
SetServoTargetAngle() 
    ├─ 立即设置 → 直接调用 set_servo_angle()
    └─ 平滑设置 → 设置 target_angle 和 step_speed
                 └─ UpdateServoMotion() 逐步逼近目标
                    └─ 周期性调用直到 is_moving=0

5.定时器中断驱动
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim_update) {  // 假设htim_update为控制更新的定时器
        UpdateServoMotion(&my_servo);
    }
}

*/

