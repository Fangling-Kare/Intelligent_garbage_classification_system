#ifndef __MY_USART_H_
#define __MY_USART_H_

#include "ALLHeader.h"
/******************************************************************************************
*                        @数据结构
******************************************************************************************/
typedef struct usart_data {
	USART_TypeDef*	USARTx;

	uint32_t		bound;

	void			(*init)(struct usart_data* self);
	void			(*tx)(struct usart_data* self,char *data_to_send);
}usart_data;

/******************************************************************************************
*                        @异或计算
******************************************************************************************/
uint8_t calculate_xor_checksum(const uint8_t *data, uint8_t length);


/******************************************************************************************
*                        @函数
******************************************************************************************/
// USART2
void __usart2_configuration(uint32_t usart_bound);
void __usart2_dma_tx_configuration(void);
void __usart2_init(uint32_t usart_bound);
	
void __usart2_send_byte(uint8_t Byte);
void __usart2_send_array(uint8_t *Arr, uint8_t Length);
void __usart2_dma_send_data(uint8_t *send_buffer , uint16_t nSendCount);
void __usart2_dma_send_byte(uint8_t byte);
void __usart2_dma_send_string(char *data_to_send);
void __usart2_dma_send_packet(uint8_t *data_to_send, uint8_t data_length);


// USART3
void __usart3_configuration(uint32_t usart_bound);
void __usart3_dma_tx_configuration(void);
void __usart3_init(uint32_t usart_bound);
	
void __usart3_send_byte(uint8_t Byte);
void __usart3_send_array(uint8_t *Arr, uint8_t Length);
void __usart3_dma_send_data(uint8_t *send_buffer , uint16_t nSendCount);
void __usart3_dma_send_string(char *data_to_send);

// USART
void __usart_init(usart_data* self);
void usart_dma_send_data(usart_data* self,char *data_to_send);
void usart_register_functions(usart_data* usart);



#endif /* __MY_USART_H_ */
