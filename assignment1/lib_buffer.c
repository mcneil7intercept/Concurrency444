#include "lib_buffer.h"

void init_buffer(Buffer *b, size_t init_size)
{
	b->array = (int *)malloc(init_size * sizeof(int));
	b->used = 0;
	b->size = init_size;
}

void insert_element(Buffer *b, int element)
{
	if(b->used == b->size) {
		b->size *= 2;
		b->array = (int *)realloc(b->array, b->size * sizeof(int));
	}
	b->array[b->used++] = element;
}

void remove_element(Buffer *b, int *topelement)
{
	//check if the buffer is empty
	if(b->used == 0){
		//set the value -1 if the buffer is empty?
		 *topelement = -1;
	}
	else{
		//retreive the last element placed in the buffer
		*topelement = b->array[b->used - 1];
		//set the element to 0 in the array and decrement the used counter
		b->array[b->used-1] = 0;
		b->used--;
	}
}

void free_buffer(Buffer *b)
{
	free(b->array);
	b->array = NULL;
	b->used = b->size = 0;
}