// *******************************************************
// 
// circBufT.c
//
// Support for a circular buffer of uint32_t values on the 
//  Tiva processor.
// P.J. Bones UCECE
// Last modified:  8.3.2017
// 
// *******************************************************

#include <stdint.h>
#include "stdlib.h"
#include "circBufT.h"

// *******************************************************
// initCircBuf: Initialise the circBuf instance. Reset both indices to
// the start of the buffer.  Dynamically allocate and clear the the 
// memory and return a pointer for the data.  Return NULL if 
// allocation fails.
void initCircBuf (circBuf_t *buffer, uint32_t size)
{
	buffer->windex = 0;
	buffer->rindex = 0;
	buffer->size = size;
	buffer->sum = 0;
	buffer->data = (int32_t *) calloc (size, sizeof(int32_t));
}
   // Note use of calloc() to clear contents.

// *******************************************************
// writeCircBuf: insert entry at the current windex location,
// advance windex, modulo (buffer size).
void writeCircBuf (circBuf_t *buffer, uint32_t entry)
{
    buffer->sum = (buffer->sum + entry) - buffer->data[buffer->windex]; //Collects a sum of the newest value minuse the oldest

	buffer->data[buffer->windex] = entry;
	buffer->windex++;
	if (buffer->windex >= buffer->size)
	   buffer->windex = 0;
}
