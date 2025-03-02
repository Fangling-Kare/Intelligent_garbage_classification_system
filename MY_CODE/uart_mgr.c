#include <string.h>
#include "usart.h"
/******************************************************************************************
*                        @宏定义和数据
******************************************************************************************/
#define CHECK_DATA_STATE 	0 	//接收数据包：该位为0不要校验位，为1要校验位
#define USART_RX_MODE_STATE	0	//USART接收模式
// 0为非协议帧模式，1为协议帧模式

#if USART_RX_MODE_STATE == 0
    // 定义全局接收缓冲区
    #define RX_BUFFER_SIZE 128
    uint8_t rx_buffer[RX_BUFFER_SIZE];
    volatile uint16_t rx_index = 0;  // volatile确保中断与主程序同步

#elif USART_RX_MODE_STATE == 1
    // 协议定义
    #define FRAME_HEADER 0xAA
    #define FRAME_TAIL   0x55
    #define MAX_FRAME_LEN 64

    typedef enum {
        RX_STATE_WAIT_HEADER,
        RX_STATE_RECEIVING,
        RX_STATE_WAIT_TAIL
    } RxState;

    // 全局状态变量
    volatile RxState rx_state = RX_STATE_WAIT_HEADER;
    uint8_t frame_buffer[MAX_FRAME_LEN];
    volatile uint8_t frame_index = 0;
    volatile uint8_t frame_ready = 0;
#endif

// 宏定义包格式
#define PACKET_HEADER       0xAA
#define PACKET_FOOTER       0x55
#define MAX_PAYLOAD_LEN     128
#define MAX_PACKET_SIZE     (1 + 1 + MAX_PAYLOAD_LEN + 1 + 1)  // HEADER + LEN + DATA + CHECKSUM + FOOTER

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
 * @brief  UART 发送完成中断回调函数（由 HAL 库调用）
 * @param  huart: 触发回调的 UART 句柄指针（如 &huart1）
 * @retval None
 * @note   - 此函数在 UART 发送完成（TX Transfer Complete）时由 HAL 库自动调用
 *         - 用户可在此函数中添加发送完成后的处理逻辑（如释放资源、触发事件标志等）
 **/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        
    }
}
/**
 * @brief  UART 接收完成中断回调函数（由 HAL 库调用）
 * @param  huart: 触发回调的 UART 句柄指针（例如 &huart1）
 * @retval None
 * @note   
 *         
 **/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
#if USART_RX_MODE_STATE == 0
    if (huart->Instance == USART1) {  // 更安全的判断方式
        //回显接收到的字节（非阻塞式DMA发送）
        HAL_UART_Transmit_DMA(huart, &rx_buffer[rx_index], 1);

        //更新缓冲区索引（环形缓冲区）
        rx_index = (rx_index + 1) % RX_BUFFER_SIZE;

        //重启接收（指向下一个缓冲区位置）
        HAL_UART_Receive_IT(huart, &rx_buffer[rx_index], 1);
    }
#elif USART_RX_MODE_STATE == 1
    if (huart->Instance == USART1) {
        uint8_t received_byte = rx_buffer[0];  // 假设每次接收1字节
        switch (rx_state) {
        case RX_STATE_WAIT_HEADER:
            if (received_byte == FRAME_HEADER) {
                frame_index = 0;
                rx_state = RX_STATE_RECEIVING;
            }
            break;
        case RX_STATE_RECEIVING:
            frame_buffer[frame_index++] = received_byte;
            if (frame_index >= MAX_FRAME_LEN) {
                // 缓冲区溢出，重置状态
                rx_state = RX_STATE_WAIT_HEADER;
            }
            break;
        case RX_STATE_WAIT_TAIL:
            if (received_byte == FRAME_TAIL) {
                frame_ready = 1;  // 通知主循环处理完整帧
                rx_state = RX_STATE_WAIT_HEADER;
            }
            break;
        }
        // 重启接收（单字节模式）
        HAL_UART_Receive_IT(huart, rx_buffer, 1);
    }
#endif
}


// 错误回调函数
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // 处理错误（如溢出、噪声等）
        if(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) {
            __HAL_UART_CLEAR_OREFLAG(huart);
        }
        // 重新启动接收
        HAL_UART_Receive_IT(huart, rx_buffer, 1);
    }
}

/******************************************************************************************
*                        @如何使用
******************************************************************************************/
/*
// 示例：发送有效载荷数据
uint8_t payload[] = {0x01, 0x02, 0x03};
HAL_StatusTypeDef status = USART2_DMA_SendPacket(&huart2, payload, sizeof(payload));

// 检查发送状态
if (status == HAL_OK) {
    // 发送成功启动
} else if (status == HAL_BUSY) {
    // 上一次传输未完成，需要处理重试逻辑
}
*/
