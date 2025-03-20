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
  * @param  angle: 目标角度（0.0~270.0度）
  * @retval 无
  */
 void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t Channel, float angle)
 {
     // 角度限幅（0.0~270.0度）
     if (angle < 0.0f) angle = 0.0f;
     else if (angle > 270.0f) angle = 270.0f;
     
     // 将角度线性映射到脉宽（500~2500μs）
     float pulse_width_us = 500.0f + (angle * 2000.0f) / 270.0f;
     
     // 根据定时器配置计算CCR值
     // 定时器时钟 = 84MHz/(84) = 1MHz → 1μs/tick
     uint16_t ccr_value = (uint16_t)(pulse_width_us);
     
     // 设置捕获比较寄存器
     __HAL_TIM_SET_COMPARE(htim, Channel, ccr_value);
 }

// 上面的舵机
void servo_up_set_angel(float angle)
{
    set_servo_angle(&htim4,TIM_CHANNEL_2,angle);
}
// 上面的舵机
void servo_dowm_set_angel(float angle)
{
    set_servo_angle(&htim4,TIM_CHANNEL_1,angle);
}


// 归中
void servo_return(void)
{
    servo_up_set_angel(UP_FLAT);
    

}
// 右下
void servo_else_waste(void)
{

}
// 左上
void servo_recyclable(void)
{

}
// 左下
void servo_hazardous(void)
{

    
}
// 右上
void servo_kitchen(void)
{

}
/******************************************************************************************
*                        @如何使用
******************************************************************************************/
