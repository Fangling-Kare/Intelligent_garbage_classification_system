/***************************************************************************************************************/ 
#include "ALLHeader.h"

/******************************************************************************************
*                        @宏定义
******************************************************************************************/
#define true    1
#define false   0
/******************************************************************************************
*                        @硬件实现层
******************************************************************************************/
//脉宽变化比较及速度控制
void pwm_servo_duty_compare(PWM_servo_object_TypeDef *self)   
{
    // 根据新设置的目标重新计算舵机控制参数
    if(self->duty_changed) {
        self->duty_changed = false;
        self->inc_times = self->duration / 20; // 计算需要递增的次数
        if(self->target_duty > self->current_duty) { // 计算总的位置变换量
            self->duty_inc = (float)(-(self->target_duty - self->current_duty));
        } else {
            self->duty_inc = (float)(self->current_duty - self->target_duty);
        }
        self->duty_inc /= (float)self->inc_times; //计算每控制周期位置增量 
        self->is_running = true;  // 舵机开始动作
    }
	// 需要控制舵机转动以到达新的位置
    if(self->is_running) {
        --self->inc_times;
        if(self->inc_times == 0) {
            self->current_duty = self->target_duty;   //最后一次递增就直接将设定值赋给当前值，保证最总位置正确
            self->is_running = false; //到达设定位置，舵机停止运动
        } else {
            self->current_duty = self->target_duty + (int)(self->duty_inc * self->inc_times);
        }
    }
    self->duty_raw = self->current_duty + self->offset; // 动作要加上舵机偏差
    

    TIM_SetCompare1(TIM8,self->duty_raw);	// 关键输出函数
}
void pwm_servo_set_position (PWM_servo_object_TypeDef *self, uint32_t duty, uint32_t duration)
{
    duration = duration < 20 ? 20 : (duration > 30000 ? 30000 : duration); //限制最短/最长运动时间
	duty = duty > 2500 ? 2500 : (duty < 500 ? 500 : duty); //限制脉宽最大/最小值
    self->target_duty = duty;
    self->duration = duration;
    self->duty_changed = true; //标记目标位置发送变换, 让 pwm_servo_duty_compare 计算新的运动参数
}

void pwm_servo_set_offset(PWM_servo_object_TypeDef *self, int offset)
{
    offset = offset < -100 ? -100 : (offset > 100 ? 100 : offset); //限制最小/最大偏差, 不同舵机极限会不同， 但是100是一个不错的选择
    self->offset = offset;
}

void pwm_servo_object_init(PWM_servo_object_TypeDef *obj)
{
    memset(obj, 0, sizeof(PWM_servo_object_TypeDef));
    obj->current_duty = 1500; // 默认位置
    obj->duty_raw = 1500;     // 默认实际脉宽
}
/******************************************************************************************
*                        @软件抽象层
******************************************************************************************/

/******************************************************************************************
*                        @如何使用
******************************************************************************************/
/*
pwm_servo_set_position(pwm_servos[2], 500, 800); 
*/

