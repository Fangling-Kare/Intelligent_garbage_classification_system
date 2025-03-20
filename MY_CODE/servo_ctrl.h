#ifndef __SERVO_CTRL_H_
#define __SERVO_CTRL_H_

#include "ALLHeader.h"

#define UP_FLAT         90.0   //上舵机水平 
#define UP_SKID         30.0   //上舵机正抛
#define UP_ANTI_SKID    170.0  //上舵机反抛

#define DOWM_FLAT       70.0   //下舵机水平
#define DOWM_LEFT       110.0  //下舵机左侧——可回收垃圾侧
#define DOWM_RIGHT      20.0   //下舵机右侧

void servo_up_set_angel(float angle);
void servo_dowm_set_angel(float angle);


void servo_init(void);
void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t Channel, float angle);
void servo_return(void);
void servo_else_waste(void);
void servo_recyclable(void);
void servo_hazardous(void);
void servo_kitchen(void);

#endif /* __SERVO_CTRL_H_ */
