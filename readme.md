修改时间：20250228_1940

修改人：yypskaren
## 功能总览
### 已实现的功能
1. LED接口化
2. USART接口化,DMA发送,中断接收
3. TIM4定时器中断
4. USART收发数据包并解析
5. 串口屏控制
6. TIM输出PWM

### 待优化的功能
1. USART收发数据包,解析

### 待实现的功能
1. 舵机驱动
2. 电机驱动
3. 串口屏
4. 时间片
5. 二轴舵机控制

## 如何使用
### LED

led0——红灯；led1——绿灯
1. 初始化：
- led_init();
2. 开灯：
- led0.on(&led0);
- led1.on(&led1);
3. 关灯：
- led0.off(&led0);
- led1.off(&led1);
4. 翻转：
- led0.toggle(&led0);
- led1.toggle(&led1);

### key

key0 key_up 下拉：按键未按下时，GPIO读取为低电平；按下时，读取为高电平。

### USART
1. 初始化：
- usart_init();
1. 发送：
- usart2.tx(&usart2,"111");
- usart3.tx(&usart3,"234");



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