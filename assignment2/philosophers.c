#define NUM_THREADS 5

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>	// required for srand
#include <stdlib.h>	// required for srand

void *table(void *philosopher);
void think(void *philosopher);
void get_forks(void *philosopher);
void eat(void *philosopher);

sem_t mutex;

int main()
{
	pthread_t threads[NUM_THREADS];
	sem_init(&mutex, 0, 2);
	char *philosophers[] = {"Aristotle", "Plato", "Rene Descartes", "Confucius", "John Locke"};
	int num_philosophers = 5;

	int rc;
	long t;
	for (t = 0; t < num_philosophers; t++) {
		rc = pthread_create(&threads[t], NULL, table, (void *)philosophers[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL);
}

void *table(void *philosopher)
{
	char *ph = (char *) philosopher;
	printf("%s has arrived at the table\n", ph);

	think((void *)ph);
	get_forks((void *)ph);

	pthread_exit(NULL);
}

void get_forks(void *philosopher)
{
	char *ph = (char *) philosopher;

	sem_wait(&mutex);
	printf("%s got two forks\n", ph);
	eat((void *)ph);
	sem_post(&mutex);
	printf("%s returned two forks\n", ph);
}

void think(void *philosopher)
{
	char *ph = (char *) philosopher;
	srand(time(NULL));

	int think_time = rand() % 20 + 1;
	printf("%s is thinking for %d seconds\n", ph, think_time);
	sleep(think_time);
}

void eat(void *philosopher)
{
	char *ph = (char *) philosopher;
	srand(time(NULL));

	int eat_time = rand() % 9 + 2;
	printf("%s is eating for %d seconds\n", ph, eat_time);
	sleep(eat_time);
}
