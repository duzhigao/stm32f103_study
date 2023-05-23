#ifndef _LED_H_
#define _LED_H_
#include "typedef.h"

#define A(address,n) (((address&0xF0000000)+0x2000000)+((address&0xFFFFF)<<5)+(n<<2))  //算出对应位段别名区春存储单元的地址
#define BA(address,n) (*((volatile unsigned int *)(A(address,n))))	 //访问对应别名区的存储单元
#define PB_OUT(n) BA(0x40010C0C,n)
#define LED_RED PB_OUT(0)
#define LED_BLUE PB_OUT(5)


void LED_Init(void);
#endif