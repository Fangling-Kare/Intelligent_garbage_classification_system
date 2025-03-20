#include "stm32f4xx_hal.h" 
#include "usart.h"
#include "servo_ctrl.h"

#define ELSE_WASTE  0xAA
#define HAZARDOUS   0xBB
#define KITCHEN     0xCC
#define RECYCLABLE  0xDD

// 舵机处理
void throw_process(uint8_t garbage_types)
{
    switch (garbage_types){
    case ELSE_WASTE:
        servo_else_waste();
        break;
    case HAZARDOUS:
        servo_hazardous();
        break;
    case KITCHEN:
        servo_kitchen();
        break;
    case RECYCLABLE:
        servo_recyclable();
        break;
    default:
        servo_return();
        break;
    }
}

// 发送准备好的指令
void usart_send_ok(void)
{
    static uint8_t data = 0xEE; 
    HAL_UART_Transmit_DMA(&huart3, &data, 1);
}


