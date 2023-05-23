#ifndef _UART_H_
#define _UART_H_
#include "typedef.h""
void Inital_Uart1(void);
void Send_Byte(uint8_t byte);
void Send_Data(uint8_t* data,uint16_t size);
uint8_t Receive_Byte(uint8_t *byte);
uint16_t Receive_Data(uint8_t *data);
void Inital_Recieve_Uart_IRQ(void);
uint16_t Get_Size();
vu8 * Get_Receive_Data();
void Set_Size(vu16 set_size);
#endif
