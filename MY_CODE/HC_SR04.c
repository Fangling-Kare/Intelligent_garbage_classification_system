#include "tim.h"
#include "HC_SR04.h"
#include "ic_detection.h"

volatile uint8_t edge_state = 0;
uint32_t start_time;
uint32_t end_time;
uint8_t update_flag;
uint32_t pulse_width = 0;
float HC_distance;
/**
  * @brief  单脉冲触发：85us后有一个15us的脉冲
  * @param  无
  * @retval 无
  */
void HC_Start(void)
{
    HAL_TIM_OnePulse_Start(&htim13, TIM_CHANNEL_1);
}
/**
  * @brief  外部中断回调：计算距离
  * @param  无
  * @retval 无
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_6)
    {
        ic_exti();
    }
    if(GPIO_Pin == GPIO_PIN_11)
    {
        if(edge_state == 0) // 上升沿：记录起始时间
        {
            start_time = TIM8->CNT;
            edge_state = 1;
        }else // 下降沿：计算时间差
        {
            end_time = TIM8->CNT;
            edge_state = 0;
            // 计算高电平时间（考虑溢出）
            pulse_width = end_time - start_time;
            if (end_time < start_time) {
                pulse_width += 0x10000; // 16位定时器补码修正
            }
            //pulse_width += overflow_count * 0x10000; // 溢出修正

			HC_distance = pulse_width * 0.01715;

            update_flag = 1;    // 设置数据就绪标志
        }
    }
}
