#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMBERS 10
#define BUFFER_LEN 256

int buffer[5];
sem_t semaphore;

void *producer(void *data) {

	int *num;
	int fill_count;
	struct timespec ts;
	
	ts.tv_sec = 0;	
	num = (int *)data;
	
	for(int i = 0; i < NUMBERS; i++) {		
		
		sem_wait(&semaphore);	
		
		while(1) {
		
			fill_count = 0;
			
			for(int j = 0; j < 5; j++) {
				
				if(buffer[j] != 0) {
				
					fill_count++;
				}
			}
						
			if(fill_count < 5) {
			
				break;			
			}			
			
			sem_post(&semaphore);
			sem_wait(&semaphore);	
		}
		
		buffer[fill_count] = num[i];
		printf("Produced %d\n", buffer[fill_count]);
		sem_post(&semaphore);	
		ts.tv_nsec = 500000000L + (rand() % 2000000000L);
		nanosleep(&ts, NULL);
	}
	
	pthread_exit(NULL);
}

void *consumer(void *unused) {

	int fill_count;	
	struct timespec ts;
	
	ts.tv_sec = 0;
	
	for(int i = 0; i < NUMBERS; i++) {
		
		sem_wait(&semaphore);	
		
		while(1) {
		
			fill_count = 0;
			
			for(int j = 0; j < 5; j++) {
				
				if(buffer[j] != 0) {
				
					fill_count++;
				}
			}
			
			if(fill_count > 0) {
			
				break;			
			}			
			
			sem_post(&semaphore);
			sem_wait(&semaphore);	
		}
		
		printf("Consumed %d\n", buffer[fill_count-1]);
		buffer[fill_count-1] = 0;	
		sem_post(&semaphore);
		ts.tv_nsec = 500000000L + (rand() % 2000000000L);
		nanosleep(&ts, NULL);
	}
	
	pthread_exit(NULL);
}

int main(void) {
	
	int num[NUMBERS];
	char input_buffer[BUFFER_LEN];
	pthread_t thread_id[2];
	pthread_attr_t attr;	
	srand(time(NULL));
	
	for(int i = 0; i < NUMBERS; i++) {
	
		printf("Enter input #%d: ", i + 1);
		
		fgets(input_buffer, BUFFER_LEN, stdin);
		num[i] = atoi(input_buffer);		
		memset(input_buffer, 0, BUFFER_LEN);
	}

	sem_init(&semaphore, 0, 1);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	if(pthread_create(&thread_id[0], &attr, producer, num)) {
	
		printf("ERROR: Unable to create producer thread\n");
		return -1;
	}
	
	if(pthread_create(&thread_id[1], &attr, consumer, NULL)) {
	
		printf("ERROR: Unable to create consumer thread\n");
		return -1;
	}

	for(int i = 0; i < 2; i++) {
		
		if(pthread_join(thread_id[i], NULL)) {
		
			printf("ERROR: Unable to join thread %d\n", i + 1);
			return -1;
		}
	}

	for(int i = 0; i < 5; i++) {
	
		printf("Buffer value at index %d=%d\n", i, buffer[i]);
	}
	
	pthread_attr_destroy(&attr);
	sem_destroy(&semaphore);
	
	return 0;
}
