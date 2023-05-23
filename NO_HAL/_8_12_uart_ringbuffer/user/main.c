#include "sys.h"
#include "uart.h"
#include "ring_buffer.h"

extern ring_buffer ring;
int main(void)
{
	System_Clock_Init();
	NVIC_Config(37,4,3,0);//NVICʹ��UART1�����жϺ��ж�
	Inital_Uart1();
	
//	/*���Է���*/
//	uint8_t i = 5;
//	uint8_t data[20] ={'A','B','C','D','\r','\n'};
//	while(i--)
//		Send_Data(data,6);
//	while(1);
//	
//	/*���Խ���*/
//	uint16_t size = 0;
//	uint8_t receive_data[100] ={0};
//	while(1)
//	{		size = Receive_Data(receive_data);
//		if(size > 0)
//			Send_Data(receive_data,size);
//	}
	/*�����жϽ���*/
	unsigned char data =0;
	while(1){
		if(ring_buffer_read(&data,&ring) != -1)
		{
			Send_Data(&data,1);
		}
			
	}
	
}