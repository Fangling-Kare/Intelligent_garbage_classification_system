#ifndef __MY_SERVOS_H_
#define __MY_SERVOS_H_


/******************************************************************************************
*                        @硬件实现层
******************************************************************************************/
typedef struct PWM_servo_object  PWM_servo_object_TypeDef;
struct PWM_servo_object {
    int id;             //舵机ID 
    int offset;         //舵机偏差
    int target_duty;    //目标脉宽
    int current_duty;   //当前脉宽
    int duty_raw;       //机器脉宽，最终写入定时器的脉宽，包含了offset == current_duty + offset 

    /* 速度控制需要的变量 */
    uint32_t duration;  //舵机从当前角度运动到指定角度的时间，也就是控制速度 单位:ms
    float duty_inc;     //每次位置更新的脉宽增量
    int  inc_times;     //需要递增的次数
    bool is_running;    //舵机是否在转动过程中
    bool duty_changed;  //脉宽是否被改变
};

void pwm_servo_duty_compare(PWM_servo_object_TypeDef *self);
//舵机脉宽控制
//计算脉宽变化、当前速度所需要的脉宽并实现控制，需要50ms调用一次
void pwm_servo_set_position (PWM_servo_object_TypeDef *self, uint32_t duty, uint32_t duration);
//设置舵机角度
//用于设置舵机的角度，实际只是设置舵机对象的几个内部变量，不会马上实际控制舵机，实际控制舵机是由 pwm_servo_duty_compare来计算并控制
void pwm_servo_set_offset(PWM_servo_object_TypeDef *self, int offset);
//设置舵机偏差
void pwm_servo_object_init(PWM_servo_object_TypeDef *obj);
/******************************************************************************************
*                        @软件抽象层
******************************************************************************************/


#endif /* __MY_SERVOS_H_ */
