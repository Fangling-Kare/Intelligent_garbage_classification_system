#ifndef __SCREEN_CTRL_H_
#define __SCREEN_CTRL_H_

#include "ALLHeader.h"
extern uint8_t __screen_task_flag;
extern int task_1_number;//tn��txt��дʲô��ÿ��ֻ��һ
extern int task_1_flag;//task1ִ�е���һ�Σ�tn��,ÿ�μ�4
extern int task_2_flag;//task2ִ�е���һ�Σ���tn��ÿ�μ�4
extern int task_3_flag;//task3ִ�е���һ�Σ���tn��ÿ�μ�4
extern int task_4_flag;//task4ִ�е���һ�Σ���tn��ÿ�μ�4
extern int task_finsh_flag;
void __screen_send_data(const char *data_to_send);
void __screen_txt(int txt_num,int data_to_send);
void __screen_page(uint8_t page_id);
void __screen_success(int i);
void __screen_task_fully_loaded(void);
void __screen_task1(void);
void __screen_task2(void);
void __screen_task3(void);
void __screen_task4(void);
void __screen_task(void);

#endif /* __SCREEN_CTRL_H_ */

