/***************************************************************************************************************/ 
#include "ALLHeader.h"

/******************************************************************************************
*                        @宏定义
******************************************************************************************/

/******************************************************************************************
*                        @TIM2 —— 直流电机
******************************************************************************************/
/*
void __tim8_configuration(uint16_t arr,uint16_t psc)
{		 					 
	GPIO_InitTypeDef            GPIO_InitStructure      = {0};
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure   = {0};
	TIM_OCInitTypeDef           TIM_OCInitStructure     = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM2,ENABLE);  	//TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能GPIOC时钟	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM8);   //GPIOC6复用为定时器8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;               //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);                   //初始化PC9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                    //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                       //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);             //初始化定时器8
	
	//初始化TIM2 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);                      //根据T指定的参数初始化外设TIM2OC1

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);             //使能TIM2在CCR1上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM2,ENABLE); //使能TIM2 
	
	TIM_Cmd(TIM2, ENABLE);            //使能TIM2					  
}  
*/

/******************************************************************************************
*                        @TIM3 —— 步进电机
******************************************************************************************/

/******************************************************************************************
*                        @TIM4 —— 中断
******************************************************************************************/
void __tim4_configuration(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure   = {0};
	NVIC_InitTypeDef        NVIC_InitStructure          = {0};
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);             ///使能TIM4时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	                //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                    //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);              //初始化TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                        //允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE);                                           //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;                   //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;      //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;             //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
        //led1.toggle(&led1);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
/******************************************************************************************
*                        @TIM9 & TIM8 —— 舵机
******************************************************************************************/
void __tim8_configuration(uint16_t arr,uint16_t psc)
{		 					 
	GPIO_InitTypeDef            GPIO_InitStructure      = {0};
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure   = {0};
	TIM_OCInitTypeDef           TIM_OCInitStructure     = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能GPIOC时钟	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM8);   //GPIOC6复用为定时器8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;               //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);                   //初始化PC6
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                    //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                       //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);             //初始化定时器8
	
	//初始化TIM8 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);                      //根据T指定的参数初始化外设TIM8OC1

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);             //使能TIM8在CCR1上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM8,ENABLE); //使能TIM8 
	
	TIM_Cmd(TIM8, ENABLE);            //使能TIM8								  
}  

/******************************************************************************************
*                        @软件抽象层
******************************************************************************************/


/******************************************************************************************
*                        @如何使用
******************************************************************************************/


