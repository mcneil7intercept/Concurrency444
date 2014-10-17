#define LIMITER 20

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "buffer_lib.h"
#include <time.h>		//required for srand seeding; TESTING ONLY

void *produce(void *buffer);	//producer thread
void *consume(void *buffer);	//consumer thread

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	Buffer b;
	init_b(&b, 32);

	pthread_t producer;
	pthread_t consumer;

	if(pthread_create(&producer, NULL, produce, &b)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create(&consumer, NULL, consume, &b)) {
		fprintf(stderr, "Error creating thread\n");
		return 2;
	}

	//wait for the producer thread to finish
	if(pthread_join(producer, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 3;
	}

	///wait for the consumer thread to finish
	if(pthread_join(consumer, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 4;
	}

	print_b(&b);

	free_b(&b);
	return 0;
}

// producer thread
void *produce(void *buffer)
{
	Buffer *b = (Buffer *)buffer;
	int step = 0;

	while(step++ < LIMITER) {
		printf("producer at step %d\n", step);
		srand(time(NULL));
		int sleep_time = rand() % 7 + 3;

		Element e;
		e.value = rand();
		e.wait_time = rand() % 9 + 2;

		//LOCK
		pthread_mutex_lock(&buffer_mutex);

		insert_b(b, &e);
		
		pthread_mutex_unlock(&buffer_mutex);
		//UNLOCK

		printf("produced %d\n", e.value);
		sleep(sleep_time);
	}

	return NULL;
}

// consumer thread
void *consume(void *buffer)
{	
	Buffer *b = (Buffer *)buffer;
	int step = 0;

	while(step++ < LIMITER) {
		printf("consumer at step %d\n", step);
		Element e;
		e.value = -1;

		//LOCK
		pthread_mutex_lock(&buffer_mutex);

		remove_b(b, &e);

		pthread_mutex_unlock(&buffer_mutex);
		//UNLOCK

		if (e.value != -1) {
			sleep(e.wait_time);
			printf("consumed %d\n", e.value);
		}
	}
	return NULL;
}
