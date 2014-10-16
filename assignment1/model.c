#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib_buffer.h"

void *produce(void *buffer);
void *consume(void *buffer);

pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;
Buffer b;

int main()
{
	init_buffer(&b, 5);

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

	free_buffer(&b);
	return 0;

}

void *produce(void *buffer)
{
	Buffer *b = (Buffer *)buffer;
	pthread_mutex_lock(&the_mutex);
	printf("mutex obtained by producer\n");
	insert_element(&(*b), 3);
	insert_element(&(*b), 5);
	pthread_mutex_unlock(&the_mutex);
	printf("mutex released by producer\n");

	return NULL;
}

void *consume(void *buffer)
{	
	Buffer *b = (Buffer *)buffer;
	int element = 0;
	pthread_mutex_lock(&the_mutex);
	printf("mutex obtained in consumer\n");	
	remove_element(b, &element);
	pthread_mutex_unlock(&the_mutex);
	printf("mutex released by consumer\n");
	printf("value consumed: %d\n", element); 
	return NULL;
}
