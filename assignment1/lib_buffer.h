#ifndef LIB_BUFFER_H_
#define LIB_BUFFER_H_

#include <stdlib.h>

typedef struct {
	int *array;
	size_t used;
	size_t size;
} Buffer;

void init_buffer(Buffer *b, size_t init_size);
void insert_element(Buffer *b, int element);
void remove_element(Buffer *b, int *topelement);
void free_buffer(Buffer *b);

#endif