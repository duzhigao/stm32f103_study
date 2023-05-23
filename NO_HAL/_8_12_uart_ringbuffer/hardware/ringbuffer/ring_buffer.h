#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H


#define BUFFER_SIZE 1024
#define is_empty(buf) ((buf)->pW == (buf)->pR?1:0)
#define is_full(buf) (((buf)->pW+1)%BUFFER_SIZE == (buf)->pR?1:0)
typedef struct
{
	unsigned char buffer[BUFFER_SIZE];
	volatile unsigned int pW;
	volatile unsigned int pR;
	
}ring_buffer;

void ring_buffer_init(ring_buffer *dst_buf);
void ring_buffer_write(ring_buffer *dst_buf,unsigned char c);
int ring_buffer_read(unsigned char *c,ring_buffer *dst_buf);
#endif