
/***************************************************************************************************************/ 
#include "ALLHeader.h"

/******************************************************************************************
*                        @宏定义
******************************************************************************************/


/******************************************************************************************
*                        @垃圾处理
******************************************************************************************/
// 初始化垃圾种类
void garbage_statistics_init(GarbageStatistics *stats) 
{
    stats->harmful_garbage = 0;
    stats->food_garbage = 0;
    stats->recyclable_garbage = 0;
    stats->other_garbage = 0;
}
// 添加垃圾数量
void add_garbage(GarbageStatistics *stats, GarbageType type, int quantity) 
{
    switch (type) 
    {
        case HARMFUL_GARBAGE:
            stats->harmful_garbage += quantity;
            break;
        case FOOD_WASTE:
            stats->food_garbage += quantity;
            break;
        case RECYCLABLE_GARBAGE:
            stats->recyclable_garbage += quantity;
            break;
        case OTHER_GARBAGE:
            stats->other_garbage += quantity;
            break;
        default:
            printf("未知的垃圾类型！\n");
            break;
    }
}

