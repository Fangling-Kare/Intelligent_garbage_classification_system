#ifndef __MY_SCREEN_H_
#define __MY_SCREEN_H_


/******************************************************************************************
*                        @硬件实现层
******************************************************************************************/

/******************************************************************************************
*                        @软件抽象层
******************************************************************************************/

void __screen_send_data(char *data_to_send);
void __screen_txt(int txt_num,char *data_to_send);

#endif /* __MY_SCREEN_H_ */
