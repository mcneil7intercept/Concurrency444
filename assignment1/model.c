#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib_buffer.h"

// these includes are only necessary for testing purposes, use RDRAND for actual random number generation
#include <time.h>
#include <stdlib.h>

void *produce(void *buffer);
void *consume(void *buffer);

pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;
Buffer b;

int main()
{
	init_buffer(&b);

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

	printf("main thread working...\n\n");
	sleep(500);

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

// producer thread
void *produce(void *buffer)
{
	while(1) {
		Buffer *b = (Buffer *)buffer;
		srand(time(NULL));
		int wait_time = rand() % 7 + 3;
		int r = rand();

		sleep(wait_time);

		pthread_mutex_lock(&the_mutex);
		printf("mutex obtained by producer\n");

		if (b->used == b->size) {
			printf("buffer is full\n");
		} else {
			insert_element(&(*b), r);
			printf("inserting %d into buffer\n", r);
		}
	
		pthread_mutex_unlock(&the_mutex);
		printf("mutex released by producer\n\n");
	}

	return NULL;
}

// consumer thread
void *consume(void *buffer)
{	
	Buffer *b = (Buffer *)buffer;

	while (1) {
		int element = 0;

		pthread_mutex_lock(&the_mutex);
		printf("mutex obtained in consumer\n");

		if (b->used == 0) {
			printf("buffer is empty\n");
		} else {
			remove_element(b, &element);
			printf("removed %d from buffer\n", element); 
		}

		pthread_mutex_unlock(&the_mutex);
		printf("mutex released by consumer\n\n");
	}
	
	return NULL;
}
