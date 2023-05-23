#include "ring_buffer.h"

void ring_buffer_init(ring_buffer *dst_buf)
{
	dst_buf->pR = 0;
	dst_buf->pW = 0;
}

void ring_buffer_write(ring_buffer *dst_buf,unsigned char c)
{
	int i = (dst_buf->pW + 1) % BUFFER_SIZE;
	if(i != dst_buf->pR)
	{
		dst_buf->buffer[dst_buf->pW] = c;
		dst_buf->pW = i;
	}
}

int ring_buffer_read(unsigned char *c,ring_buffer *dst_buf)
{
	if(dst_buf->pR == dst_buf->pW)
		return -1;
	*c = dst_buf->buffer[dst_buf->pR];
	dst_buf->pR = (dst_buf->pR + 1) % BUFFER_SIZE;
	return 0;
}