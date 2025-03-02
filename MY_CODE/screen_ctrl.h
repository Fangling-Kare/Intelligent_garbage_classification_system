#ifndef __SCREEN_CTRL_H_
#define __SCREEN_CTRL_H_

#include "ALLHeader.h"

void __screen_send_data(char *data_to_send);
void __screen_txt(int txt_num,int data_to_send);
void __screen_page(uint8_t page_id);
void __screen_kind(uint8_t garbage__kind);
void __screen_success(int i);


#endif /* __SCREEN_CTRL_H_ */

