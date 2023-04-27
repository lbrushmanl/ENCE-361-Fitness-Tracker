#ifndef CIRCBUFT_H_
#define CIRCBUFT_H_

// *******************************************************
// 
// circBufT.h
//
// Support for a circular buffer of uint32_t values on the 
//  Tiva processor.
// P.J. Bones UCECE
// Last modified:  7.3.2017
// 
// *******************************************************
#include <stdint.h>

// *******************************************************
// Buffer structure
typedef struct {
	uint32_t size;		// Number of entries in buffer
	uint32_t windex;	// index for writing, mod(size)
	uint32_t rindex;	// index for reading, mod(size)
	int32_t sum;       // sum of buffer
	int32_t *data;		// pointer to the data
} circBuf_t;

// *******************************************************
// initCircBuf: Initialise the circBuf instance. Reset both indices to
// the start of the buffer.  Dynamically allocate and clear the the 
// memory and return a pointer for the data.  Return NULL if 
// allocation fails.
void initCircBuf (circBuf_t *buffer, uint32_t size);

// *******************************************************
// writeCircBuf: insert entry at the current windex location,
// advance windex, modulo (buffer size).
void writeCircBuf (circBuf_t *buffer, uint32_t entry);

#endif /*CIRCBUFT_H_*/
