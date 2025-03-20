#ifndef __UART_MGR_H_
#define __UART_MGR_H_

#include "ALLHeader.h"

#define MAX_PAYLOAD_LEN     128
#define MAX_PACKET_SIZE     (1 + 1 + MAX_PAYLOAD_LEN + 1 + 1)  // HEADER + LEN + DATA + CHECKSUM + FOOTER

#define MAX_FRAME_LEN 64
#define RX_BUFFER_SIZE 128

extern uint8_t rx_buffer[RX_BUFFER_SIZE];
extern uint8_t frame_buffer[MAX_FRAME_LEN];;
extern uint8_t frame_ready;
extern uint8_t frame_index;


uint8_t calculate_xor_checksum(const uint8_t *data, uint8_t length);

void usart_init(void);

HAL_StatusTypeDef USART_DMA_SendPacket(UART_HandleTypeDef *huart, uint8_t *data, uint8_t length);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);


#endif /* __UART_MGR_H_ */
