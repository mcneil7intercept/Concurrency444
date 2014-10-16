#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int *array;
	size_t used;
	size_t size;
} Buffer;

void init_buf(Buffer *b, size_t init_size);
void insert_buf(Buffer *b, int element);
void remove_top_buf(Buffer *b, int *topelement);
void free_buf(Buffer *b);

void *produce(void *buffer);
void *consume(void *buffer);

pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;
Buffer b;

int main()
{
	init_buf(&b, 5);

	pthread_t producer_thread;
	pthread_t consumer_thread;

	if(pthread_create(&producer_thread, NULL, produce, &b)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create(&consumer_thread, NULL, consume, &b)) {
		fprintf(stderr, "Error creating thread\n");
		return 2;
	}

	printf("main thread working...\n");

	/* wait for the second thread to finish */
	if(pthread_join(producer_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 3;
	}

	/* wait for the third thread to finish */
	if(pthread_join(consumer_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 4;
	}

	int i;
	for (i = 0; i < b.size; i++) {
		printf("%d: %d\n", i, b.array[i]);
	}

	free_buf(&b);
	return 0;

}

void init_buf(Buffer *b, size_t init_size)
{
	b->array = (int *)malloc(init_size * sizeof(int));
	b->used = 0;
	b->size = init_size;
}

void insert_buf(Buffer *b, int element)
{
	if(b->used == b->size) {
		b->size *= 2;
		b->array = (int *)realloc(b->array, b->size * sizeof(int));
	}
	b->array[b->used++] = element;
}

void remove_top_buf(Buffer *b, int *topelement)
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

void free_buf(Buffer *b)
{
	free(b->array);
	b->array = NULL;
	b->used = b->size = 0;
}

void *produce(void *buffer)
{
	Buffer *b = (Buffer *)buffer;
	pthread_mutex_lock(&the_mutex);
	printf("mutex locked in producer\n");
	insert_buf(&(*b), 3);
	insert_buf(&(*b), 5);
	pthread_mutex_unlock(&the_mutex);
	printf("produced 3 & 5 into buffer\n");

	return NULL;
}

void *consume(void *buffer)
{	
	Buffer *b = (Buffer *)buffer;
	int element = 0;
	pthread_mutex_lock(&the_mutex);
	printf("mutex obtained in consumer\n");	
	remove_top_buf(b, &element);
	pthread_mutex_unlock(&the_mutex);
	printf("Value consumed: %d\n", element); 
	return NULL;
}
