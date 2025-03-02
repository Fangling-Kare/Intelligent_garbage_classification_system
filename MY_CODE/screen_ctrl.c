#include "usart.h"

void __screen_send_data(const char *data_to_send) 
{
    //安全检查
    if (data_to_send == NULL || huart2.State != HAL_UART_STATE_READY) {
        return;  // 可替换为错误处理
    }

    //计算实际数据长度（假设以'\0'结尾）
    uint16_t data_len = strlen(data_to_send);

    //发送数据（限制不超过最大包长度）
    HAL_UART_Transmit_DMA(&huart2, (uint8_t*)data_to_send, 
                         (data_len < MAX_PACKET_SIZE) ? data_len : MAX_PACKET_SIZE);
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

