修改时间：2025_

修改人：yypskaren

## 接线表

| 驱动的物品 |名字      | MCU用到的外设 | GPIO |
| ------    | ----     | ---- | ---- |
| 直流电机   || TIM2CH1 | PA5   |
|           || TIM2CH2 | PA1   |
|           || TIM2CH4 | PA3   |
| 舵机      | 圆盘舵机下| TIM4CH1 | PB6   |
|           |圆盘舵机上 | TIM4CH2 | PB7   |
| USART通信 |          | USART2_TX|  PA2 |
|           |         | USART2_RX |  PD6 |
|           |          | USART3_TX|  PB10 |
|           |         | USART3_RX |  PB11 |

## 时钟树
- system_tick:168MHz

- APB1:84MHz
    - TIM2:
    - TIM4:50Hz


## HAL库

<details>
<summary style="font-size: 17px;">USART</summary>

1. HAL_UART_Transmit(&huart1,uint8_t *pData,uint16_t Num,超时值); 
   - 阻塞式发送，
2. HAL_UART_Transmit_IT(&huart1,uint8_t *pData,uint16_t Num);
   - 非阻塞式发送，利用中断发送。参数：串口，数据地址，发送的字节数
3. HAL_UART_Transmit_DMA(&huart1,uint8_t *pData,uint16_t Num);
   - DMA发送。参数：串口，数据地址，发送的字节数。
- 注意：中断发送和DMA发送需要注意发送间隔
</details>

<details>
<summary style="font-size: 17px;">TIM</summary>

1. HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_x);
    - 启动TIM
    - TIM_CHANNEL_x：通道x，TIM_CHANNEL_ALL:代表全部通道
2. __HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_x, ccr_value);
    - 修改CCR值，也可以TIMx->CCR1 = 150;
</details>

## 功能
### USART
1. DMA发送:
    1. 示例：
    - uint8_t pData[] = "Hello";
    - uint16_t Num = sizeof(pData) - 1;
    - HAL_UART_Transmit_DMA(&huart1, pData, Num);
1. 中断接收:
   1. 非数据包模式:USART_RX_MODE_STATE == 0
   2. 数据包模式:USART_RX_MODE_STATE == 1

### TIM
- 频率：Fpwm = 84M(APB时钟) / ((arr+1)*(psc+1))(单位：Hz)

- 占空比：duty circle = TIMx->CCR1 / arr(单位：%)

- APB1：TIM2、TIM4，84MHz

### 舵机-Servo
- TIM4:84-1分频，ARR:20000-1,50Hz
- 角度控制：PWM脉宽500-2500，对应舵机角度正负135

    |角度   | 脉宽   |CCR   | 
    |----   | ------| ---- |
    |-135   | 0.5   | 500  |
    |-90    | 1     | 100  |
    |-45    | 1.333 | 1333 |
    |0      | 1.5   | 1500 |
    |45     | 1.666 | 1666 |
    |90     | 2000  | 2000 |
    |135    | 2500  | 2500 |


### 电机-Motor

## 如何使用
### USART
1. DMA发送：
- HAL_UART_Transmit_DMA(&huart1,uint8_t *pData,uint16_t Num);
2. DMA发送协议帧：
- USART_DMA_SendPacket(UART_HandleTypeDef *huart, uint8_t *data, uint8_t length);
### 舵机
1. 





