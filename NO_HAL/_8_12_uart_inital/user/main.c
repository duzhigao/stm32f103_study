#include "sys.h"
#include "uart.h"
int main(void)
{
	System_Clock_Init();
	Inital_Uart1();
//	/*²âÊÔ·¢ËÍ*/
//	uint8_t i = 5;
//	uint8_t data[20] ={'A','B','C','D','\r','\n'};
//	while(i--)
//		Send_Data(data,6);
	
	/*²âÊÔ½ÓÊÕ*/
	uint16_t size = 0;
	uint8_t receive_data[40] ={0};
	while(1)
	{		size = Receive_Data(receive_data);
		if(size > 0)
			Send_Data(receive_data,size);
	}
	
}