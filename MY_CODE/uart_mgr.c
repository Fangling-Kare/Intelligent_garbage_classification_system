
#include <string.h>
#include "usart.h"
#include "uart_mgr.h"
/******************************************************************************************
*                        @宏定义和数据
******************************************************************************************/
#define CHECK_DATA_STATE 	0 	//接收数据包：该位为0不要校验位，为1要校验位
#define USART_RX_MODE_STATE	0	//USART接收模式
// 0为非协议帧模式，1为协议帧模式
uint8_t frame_ready = 0;

#if USART_RX_MODE_STATE == 0
    // 定义全局接收缓冲区
    uint8_t rx_buffer[RX_BUFFER_SIZE];
    volatile uint16_t rx_index = 0;  // volatile确保中断与主程序同步

#elif USART_RX_MODE_STATE == 1
    // 协议定义
    #define FRAME_HEADER 0x0B	// 帧头
    #define FRAME_TAIL   0x0E	// 帧尾
		
    #define FRAME_LEGTH  1		// 数据帧长度

    uint8_t received_byte[1];
    uint8_t received_data[1];
    typedef enum {
        RX_STATE_WAIT_HEADER,
        RX_STATE_RECEIVING,
        RX_STATE_WAIT_TAIL
    } RxState;

    // 全局状态变量
    volatile RxState rx_state = RX_STATE_WAIT_HEADER;
    uint8_t frame_buffer[MAX_FRAME_LEN];
    uint8_t received_data[1];
    uint8_t frame_index = 0;
#endif

// 宏定义包格式
#define PACKET_HEADER       0xAA
#define PACKET_FOOTER       0x55



/******************************************************************************************
*                        @异或计算
******************************************************************************************/
uint8_t calculate_xor_checksum(const uint8_t *data, uint8_t length) 
{
    uint8_t checksum = 0;  // 初始化异或校验值为 0

    // 遍历指定长度的数据
    for (uint8_t i = 0; i < length; i++) {
        checksum ^= data[i];  // 对每个字节进行异或运算
    }

    return checksum;  // 返回最终的异或校验值
}
/******************************************************************************************
*                        @中断接收字节数初始化
******************************************************************************************/
void usart_init(void)
{
#if USART_RX_MODE_STATE == 0
    HAL_UART_Receive_IT(&huart2, rx_buffer, 1);
    HAL_UART_Receive_IT(&huart3, rx_buffer, 1);
#elif USART_RX_MODE_STATE == 1
    HAL_UART_Receive_IT(&huart2, (uint8_t*)received_byte, 1);

#endif
}
/******************************************************************************************
*                        @发送
******************************************************************************************/
/**
  * @brief  通过USARTDMA发送协议数据包（非阻塞）
  * @param  huart: UART句柄指针
  * @param  data: 待发送的有效载荷数据
  * @param  length: 有效载荷数据长度（必须≤MAX_PAYLOAD_LEN）
  * @retval HAL_StatusTypeDef: 发送状态（HAL_OK/HAL_ERROR/HAL_BUSY）
  */
HAL_StatusTypeDef USART_DMA_SendPacket(UART_HandleTypeDef *huart, uint8_t *data, uint8_t length)
{
    // 参数校验
    if (length > MAX_PAYLOAD_LEN || data == NULL) {
        return HAL_ERROR;
    }
    // 检查DMA传输状态
    if (huart->hdmatx->State != HAL_DMA_STATE_READY) {
        return HAL_BUSY;
    }
    // 构造协议包
    uint8_t packet[MAX_PACKET_SIZE];
    uint8_t *ptr = packet;
    // Header
    *ptr++ = PACKET_HEADER;
    // Length
    *ptr++ = length;
    // Payload
    memcpy(ptr, data, length);
    ptr += length;
    // Checksum（从Length字节开始计算）
    uint8_t checksum = 0;
    for (uint8_t i = 1; i < length + 2; i++) {  // 校验范围：Length+Payload
        checksum ^= packet[i];
    }
    *ptr++ = checksum;
    // Footer
    *ptr++ = PACKET_FOOTER;
    // 启动DMA传输
    return HAL_UART_Transmit_DMA(huart, packet, MAX_PACKET_SIZE);
}
/******************************************************************************************
*                        @回调函数
******************************************************************************************/
/**
 * @brief  UART 接收完成中断回调函数（由 HAL 库调用）
 * @param  huart: 触发回调的 UART 句柄指针（例如 &huart1）
 * @retval None
 * @note   
 *         
 **/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
#if USART_RX_MODE_STATE == 0
    if (huart->Instance == USART2) {  // 更安全的判断方式
        // 处理接收到的数据（示例：回显数据）
        //HAL_UART_Transmit(&huart2, rx_buffer, 1, 100);
        frame_ready = 1;  
        // 重新启动接收中断，以便持续接收数据
        HAL_UART_Receive_IT(&huart2, rx_buffer, 1);
    }
#elif USART_RX_MODE_STATE == 1
    if (huart->Instance == USART2) {
        uint8_t received_data[1];
        received_data[0] = received_byte[0];
        switch (rx_state) {
        case RX_STATE_WAIT_HEADER:
            if (received_byte[0] == FRAME_HEADER) {
                frame_index = 0;
                rx_state = RX_STATE_RECEIVING;
            }
            break;
        case RX_STATE_RECEIVING:
            frame_buffer[frame_index++] = received_data[0];
            if (frame_index == frame_index)
            {
                rx_state = RX_STATE_WAIT_TAIL;
            }
            if (frame_index >= MAX_FRAME_LEN) {
                // 缓冲区溢出，重置状态
                rx_state = RX_STATE_WAIT_HEADER;
            }
            break;
        case RX_STATE_WAIT_TAIL:
            if (received_data[0] == FRAME_TAIL) {
                frame_ready = 1;  // 通知主循环处理完整帧
                rx_state = RX_STATE_WAIT_HEADER;
            }
            break;
        }
        // 重启接收（单字节模式）
        HAL_UART_Receive_IT(&huart2, received_byte, 1);
    }
#endif
}


/******************************************************************************************
*                        @如何使用
******************************************************************************************/

