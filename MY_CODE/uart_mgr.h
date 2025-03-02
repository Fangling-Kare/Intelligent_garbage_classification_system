#ifndef __UART_MGR_H_
#define __UART_MGR_H_

#include "ALLHeader.h"

uint8_t calculate_xor_checksum(const uint8_t *data, uint8_t length);

HAL_StatusTypeDef USART_DMA_SendPacket(UART_HandleTypeDef *huart, uint8_t *data, uint8_t length);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);

#endif /* __UART_MGR_H_ */
