/***************************************************************************************************************/ 
#include "ALLHeader.h"

/******************************************************************************************
*                        @宏定义
******************************************************************************************/

/******************************************************************************************
*                        @屏幕操作
******************************************************************************************/
void __screen_send_data(char *data_to_send)
{
    __usart2_dma_send_string(data_to_send);
}
//修改txt文本
void __screen_txt(int txt_num,int data_to_send)
{
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "t%d.txt=\"%d\"\xff\xff\xff", txt_num, data_to_send);
    __screen_send_data(buffer);
}

//翻页
void __screen_page(uint8_t page_id)
{
	
    if (page_id == 0)
    {
        __screen_send_data("page0\xff\xff\xff");
    }
    else if (page_id == 1)
    {
        __screen_send_data("page1\xff\xff\xff");
    }    
}



void __screen_kind(uint8_t garbage__kind)
{
    switch (garbage__kind)
    {
        case 0:
            __screen_send_data("t4.txt=\"有害垃圾\"\xff\xff\xff");
            break;
        case 1:
            __screen_send_data("t4.txt=\"厨余垃圾\"\xff\xff\xff");
            break;
        case 2:
            __screen_send_data("t4.txt=\"可回收垃圾\"\xff\xff\xff");
            break;
        case 3:
            __screen_send_data("t4.txt=\"其他垃圾\"\xff\xff\xff");
            break;
        default:
            __screen_send_data("t4.txt=\"出错\"\xff\xff\xff");
            break;
    }
}

void __screen_success(int i)
{
    switch (i)
    {
        case 0:
            __screen_send_data("t6.txt=\"否\"\xff\xff\xff");
            break;
        case 1:
            __screen_send_data("t6.txt=\"是\"\xff\xff\xff");
            break;
        default:
            __screen_send_data("t6.txt=\"Error\"\xff\xff\xff");
            break;
    }
}




/******************************************************************************************
*                        @如何使用
******************************************************************************************/


