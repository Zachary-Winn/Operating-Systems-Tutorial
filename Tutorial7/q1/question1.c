#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int queueSize =0;


struct proc{
	char name[255];
	int priority;
	int pid;
	time_t runtime;
};

struct queue{
	struct proc process;
	struct queue *next;
};

struct queue * current, next,new[254],head;

int push(struct proc pushedProcess){

	if(queueSize==0){

		new[queueSize].next = NULL;
		new[queueSize].process=pushedProcess;
		head = new[queueSize];
		++queueSize;

	}
	else{
		current = &head;
		//while current isnt the last element move to the next element
		while(current->next != NULL){
			current = current->next;
		}
		//after current reached the last element, current points to the new[queueSize] element

		current->next = &new[queueSize];
		new[queueSize].process = pushedProcess;
		new[queueSize].next = NULL;
		++queueSize;
	}

	return 0;
}   



int main(void){




	FILE *fp;
	const char delim[2] =",";
	char buffer[255];
	char * token;

	printf("opening file\n");
	fp = fopen("processes.txt", "r");
	int charCounter = 0,c;

	struct proc listOfProcs[10];


	for(int i = 0;i<10;i++){// for each line in the text file
		while((c = fgetc(fp) )!= '\n'){
			//while it is not the next line keep grabbing character
			buffer[charCounter++] = c;
		}
		charCounter=0;
		//when a full line has been placed into the buffer split the line by , delim
		token = strtok(buffer,delim);


		//for each of the 4 elemets of the proc structure
		for(int j =0;j<4;j++){
			switch(j){
				case 0: strcpy(listOfProcs[i].name,token);break;
				case 1: listOfProcs[i].priority = atoi(token);break;
				case 2: listOfProcs[i].pid = atoi(token);break;
				case 3: listOfProcs[i].runtime = atoi(token);break;
				default:printf("this should not be seen\n");return 1;
			}
			token = strtok(NULL,delim);
		}

		//clean the buffer
		for(int c = 0; c<255;c++){
			buffer[c] = ' ';
		}

	}//end while

	printf("closing file\n");
	fclose(fp);


	/*
	   after all procs have been made begin linking them together with queue via the push method
	   */

	for(int i = 0;i<10;i++){
		push(listOfProcs[i]);
	}


	printf("iterating through queue values\n\n");

	struct queue *checker = &head;


	while(checker!=NULL){
		printf("name: %s\n",checker->process.name);
		printf("priority: %i\n",checker->process.priority);
		printf("pid: %i\n",checker->process.pid);
		printf("runtime: %li\n\n",checker->process.runtime);


		checker = checker->next;

	}

	return 0;
}
