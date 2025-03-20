#ifndef __SCREEN_CTRL_H_
#define __SCREEN_CTRL_H_

#include "ALLHeader.h"
extern uint8_t __screen_task_flag;
extern int task_1_number;//tn的txt该写什么，每次只加一
extern int task_1_flag;//task1执行到哪一次（tn）,每次加4
extern int task_2_flag;//task2执行到哪一次，（tn）每次加4
extern int task_3_flag;//task3执行到哪一次，（tn）每次加4
extern int task_4_flag;//task4执行到哪一次，（tn）每次加4
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

