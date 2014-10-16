#ifndef LIB_BUFFER_H_
#define LIB_BUFFER_H_

#include <stdlib.h>

typedef struct {
	int *array;
	size_t used;
	size_t size;
} Buffer;

typedef struct {
	int value;
	int wait_time;
} Element;

void init_buffer(Buffer *b);
void insert_element(Buffer *b, int element);
void remove_element(Buffer *b, int *topelement);
void free_buffer(Buffer *b);

#endif