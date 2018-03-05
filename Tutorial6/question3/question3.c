#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



sem_t semmy;
int moving_sum[5]= {0,0,0,0,0}; 



struct numdex{
	int number;
	int index;
};


//thread called, pass numdex struct to
void *factorial(void *arg)
{

	struct numdex* a = (struct numdex *) arg;

	int numb = (int)(a->number);
	int ind = (int)(a->index);


	int value = 1;
	for(int i = numb; i > 0; i-- ){
		value *= i; 
	}
	if(ind !=0){

		while(1){
			sem_wait(&semmy); 
			if(moving_sum[ind-1]>0){
				value += moving_sum[ind-1];
				moving_sum[ind] = value;
				sem_post(&semmy);		
				break;
			}
			sem_post(&semmy);

		}
	}
	else{
		moving_sum[ind] = value;
	}


	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t threads[5]; // 5 possible threads

	struct numdex userNums[5];

	for(int i = 0; i<5 ;i++){		//get all user numbers
		printf("input number %i: ",i);	
		scanf("%d",&userNums[i].number);
		userNums[i].index = i;

	}

	//general semaphore error handling
	if(sem_init(&semmy, 0, 1)) 
	{
		printf("Could not initialize a semaphore\n");
		return -1;
	}

	//create threads with error handling
	printf("starting threads\n");
	for(int i = 0; i < 5; ++i)
	{
		if(pthread_create(&threads[i], NULL, factorial, (void *)&(userNums[i])))
		{
			printf("Could not create thread %d\n", i);
			return -1;
		}
	}
	printf("all threads created\n");
	for(int i = 0; i < 5; ++i)
	{
		pthread_join(threads[i], NULL);

	}
	printf("all threads finished\n");


	printf("\nContents of moving_sum:\n");

	for(int i = 0; i<5;i++){
		printf("[ %i ]",moving_sum[i]);
	}
	printf("\n");
	sem_destroy(&semmy);
	return 0;
}
