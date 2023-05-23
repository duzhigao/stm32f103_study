#include "key.h"
#include "delay.h"
#include "sys.h"
//key1-PA0
//key2-PG15
//key3-PC13
//key4-PE3

/* 按键识别函数.有按键按下返回按键的值，没有按键按下返回 0；
返回的按键的值与按键的关系为：KEY1=1;KEY2=2;KEY3=3;KEY_UP=4. */ 
uint8_t Scan_Key(void)
{
	static uint8_t key_flag = 1; //按键处于弹起状态为1，按下状态为0，防止重复触发
	//static修饰的局部变量
	//如果有初始值放在data段且在初始化全局变量时初始化
	//如果没有初始值，则放在.bss段，在初始化全局变量时初始化为0

	/*如果按键刚刚处于弹起状态，但现在有按下*/
	if((KEY1 == PRESSED || KEY2 == PRESSED || KEY3 == PRESSED || KEY4 == PRESSED) && key_flag == 1)
	{
		Delay(10);//延时 10ms,消除抖动
		if(KEY1 == PRESSED || KEY2 == PRESSED || KEY3 == PRESSED || KEY4 == PRESSED)
		{
			key_flag = 0;//间隔 10ms 消除抖动后还按下，说明是真的按下
			if(KEY1 == PRESSED) return 1;
			if(KEY2 == PRESSED) return 2;
			if(KEY3 == PRESSED) return 3;
			if(KEY4 == PRESSED) return 4;
			
		}
	}
	
	/*对按键弹起进行判断，如果满足刚才是按下，现在是弹出，说明弹起了*/
	if((KEY1 == NO_PRESSED || KEY2 == NO_PRESSED || KEY3 == NO_PRESSED || KEY4 == NO_PRESSED) && key_flag == 0)
	{
		Delay(10);
		if(KEY1 == NO_PRESSED || KEY2 == NO_PRESSED || KEY3 == NO_PRESSED || KEY4 == NO_PRESSED)
			key_flag = 1;
	}
	return 0;
}

void KEY_Init(void)
{
	RCC_APB2ENR |=(1<<2)|(1<<4)|(1<<6)|(1<<8); //使能GPIO_A C E G 模块时钟
	GPIO_SET(GPIOA,1,0,1,0); //根据电路图浮空输入，无需上下拉电阻
	GPIO_SET(GPIOC,1<<13,0,1,0);
	GPIO_SET(GPIOE,1<<3,0,1,0);
	GPIO_SET(GPIOG,1<<15,0,1,0);
}