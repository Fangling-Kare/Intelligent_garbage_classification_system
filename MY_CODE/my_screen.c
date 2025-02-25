/***************************************************************************************************************/ 
#include "ALLHeader.h"

/******************************************************************************************
*                        @�궨��
******************************************************************************************/

/******************************************************************************************
*                        @��Ļ����
******************************************************************************************/
void __screen_send_data(char *data_to_send)
{
    __usart2_dma_send_string(data_to_send);
}
//�޸�txt�ı�
void __screen_txt(int txt_num,int data_to_send)
{
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "t%d.txt=\"%d\"\xff\xff\xff", txt_num, data_to_send);
    __screen_send_data(buffer);
}

//��ҳ
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
            __screen_send_data("t4.txt=\"�к�����\"\xff\xff\xff");
            break;
        case 1:
            __screen_send_data("t4.txt=\"��������\"\xff\xff\xff");
            break;
        case 2:
            __screen_send_data("t4.txt=\"�ɻ�������\"\xff\xff\xff");
            break;
        case 3:
            __screen_send_data("t4.txt=\"��������\"\xff\xff\xff");
            break;
        default:
            __screen_send_data("t4.txt=\"����\"\xff\xff\xff");
            break;
    }
}

void __screen_success(int i)
{
    switch (i)
    {
        case 0:
            __screen_send_data("t6.txt=\"��\"\xff\xff\xff");
            break;
        case 1:
            __screen_send_data("t6.txt=\"��\"\xff\xff\xff");
            break;
        default:
            __screen_send_data("t6.txt=\"Error\"\xff\xff\xff");
            break;
    }
}




/******************************************************************************************
*                        @���ʹ��
******************************************************************************************/


