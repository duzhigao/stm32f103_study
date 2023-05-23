#include "sys.h"
#include "uart.h"
int main(void)
{
	System_Clock_Init();
	NVIC_Config(37,4,3,0);//NVIC使能UART1类型中断号中断
	Inital_Uart1();
//	/*测试发送*/
//	uint8_t i = 5;
//	uint8_t data[20] ={'A','B','C','D','\r','\n'};
//	while(i--)
//		Send_Data(data,6);
//	while(1);
//	
//	/*测试接收*/
//	uint16_t size = 0;
//	uint8_t receive_data[100] ={0};
//	while(1)
//	{		size = Receive_Data(receive_data);
//		if(size > 0)
//			Send_Data(receive_data,size);
//	}
	/*测试中断接收*/
	while(1){
		if(Get_Size()!= 0)
		{
			Send_Data((unsigned char *)Get_Receive_Data(),Get_Size());
		}
			
	}
	
}