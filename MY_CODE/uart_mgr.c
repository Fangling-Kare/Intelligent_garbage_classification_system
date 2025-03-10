#include <string.h>
#include "usart.h"
/******************************************************************************************
*                        @宏定义和数据
******************************************************************************************/
#define CHECK_DATA_STATE 	0 	//接收数据包：该位为0不要校验位，为1要校验位
#define USART_RX_MODE_STATE	1	//USART接收模式
// 0为非协议帧模式，1为协议帧模式

#if USART_RX_MODE_STATE == 0
    // 定义全局接收缓冲区
    #define RX_BUFFER_SIZE 128
    uint8_t rx_buffer[RX_BUFFER_SIZE];
    volatile uint16_t rx_index = 0;  // volatile确保中断与主程序同步

#elif USART_RX_MODE_STATE == 1
    #define receiveDatalength 7
    uint8_t	receiveData[receiveDatalength];
    uint8_t validData[10];   // 存储有效数据的数组
    uint8_t validDataLength; // 有效数据的长度
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
*                        @中断接收字节数初始化
******************************************************************************************/
void usart_init(void)
{
#if USART_RX_MODE_STATE == 0
    HAL_UART_Receive_IT(&huart2, rx_buffer, 1);
    HAL_UART_Receive_IT(&huart3, rx_buffer, 1);
#elif USART_RX_MODE_STATE == 1
    HAL_UART_Receive_IT(&huart2, receiveData, receiveDatalength);

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
    if (huart->Instance == USART2) {  // 更安全的判断方式
        // 处理接收到的数据（示例：回显数据）
        HAL_UART_Transmit(huart, rx_buffer, 1, 100);

        // 重新启动接收中断，以便持续接收数据
        HAL_UART_Receive_IT(huart, rx_buffer, 1);
    }
#elif USART_RX_MODE_STATE == 1
    if (huart->Instance == USART2) {
        if(receiveData[0]==0x0b&&(receiveData[receiveData[1]+3])==0x0e)
        {
        validDataLength = receiveData[1];// 提取有效数据长度
        // 提取有效数据并存储到 validData 数组中
        for (uint8_t i = 0; i < validDataLength; i++){
            validData[i] = receiveData[2 + i]; // 有效数据从第 3 个字节开始
        }
        // 发送有效数据
        HAL_UART_Transmit_IT(&huart2, validData, validDataLength);
        }
       HAL_UART_Receive_IT(&huart2,receiveData,receiveDatalength);//不要动	 
    }
#endif
}


// 错误回调函数
// void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
//     if (huart->Instance == USART1) {
//         // 处理错误（如溢出、噪声等）
//         if(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) {
//             __HAL_UART_CLEAR_OREFLAG(huart);
//         }
//         // 重新启动接收
//         HAL_UART_Receive_IT(huart, rx_buffer, 1);
//     }
// }

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
