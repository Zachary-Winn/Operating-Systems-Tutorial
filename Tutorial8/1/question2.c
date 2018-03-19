#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_LEN 256
#define MEMORY 1024
#define NUM_LINES 10


typedef struct proc {

	char name[BUFFER_LEN];
	int priority;
	int pid;
	int address;
	int memory;
	int runtime;
	bool suspended;
} proc; 

typedef struct queue {

	proc *process;
	struct queue *next;
} queue;

queue *priority;
queue *secondary;

int avail_mem[MEMORY];

void push(queue **proc_queue, proc *process) {

	queue *new_node, *cur_node;
	
	new_node = (queue *)  calloc(1, sizeof(queue));
	new_node->process = process;
	
	if(*proc_queue == NULL) {
		
		*proc_queue = new_node;		
		return;
	}

	cur_node = *proc_queue;
	
	while(cur_node->next != NULL) {

		cur_node = cur_node->next;
	}
	
	cur_node->next = new_node;
}

proc *pop(queue **proc_queue) {

	proc *process;
	queue *head;
	
	if(*proc_queue == NULL) {

		return NULL;
	}
	
	head = *proc_queue;	
	process = head->process;
	*proc_queue = head->next;
	free(head);
	
	return process;
}

int find_free_block(proc *process) {

	bool available;		
	
	for(int i = 0; i < MEMORY; i++) {
		
		available = true;

		if((i + process->memory) > MEMORY) {

			break;
		}
		
		for(int j = 0; j < process->memory; j++) {

			if(avail_mem[i + j] != 0) {

				available = false;
				break;
			}
		}
		
		if(available) {
			
			return i;
		}
	}
	
	return -1;
}

int main(void) {

	FILE *fp;
	proc *process;
	int free_index;
	int status, pid;
	
	fp = fopen("processes_q2.txt", "r");
	
	if(fp == NULL) {

		printf("ERROR: Unable to open processes_q2.txt\n");
		return -1;
	}
	
	for(int i = 0; i < NUM_LINES; i++) {
		
		process = (proc *) calloc(1, sizeof(proc));
				
		fscanf(fp, "%[^,], %d, %d, %d\n", process->name, &process->priority, &process->memory, &process->runtime);
		
		process->pid = 0;
		process->address = 0;
		process->suspended = false;
				
		if(process->priority == 0) {

			push(&priority, process);
		} else {

			push(&secondary, process);
		}
	}	

	fclose(fp);	
		
	while((process = pop(&priority)) != NULL) {
		
		free_index = find_free_block(process);
		
		if(free_index == -1) {

			continue;
		}
		
		process->address = free_index;

		for(int i = 0; i < process->memory; i++) {

			avail_mem[process->address + i] = 1;
		}	
		
		pid = fork();

		//child
		if(pid == 0) {
				
			execl("./process", "./process", NULL);

			printf("ERROR: Unable to call exec: %s\n", process->name);
			exit(-1);
			
		} else if(pid == -1) {

			printf("ERROR: Unable to create child process\n");
			exit(-1);
			
        //parent   
		} else {			
						
			process->pid = pid;

			printf("%s, %d, %d, %d, %d\n", process->name, process->priority, process->pid, process->memory, process->runtime);

			sleep(process->runtime);
			
			if(kill(process->pid, SIGINT) == -1) {

				printf("ERROR: Unable to signal child process: %s\n", process->name);
				exit(-1);
			}
			
			if(waitpid(process->pid, &status, 0) != pid) {

				printf("ERROR: Unable to wait for child process: %s\n", process->name);
				exit(-1);
			}
						
			for(int i = 0; i < process->memory; i++) {

				avail_mem[process->address + i] = 0;
			}			
		}
	}
	
	while((process = pop(&secondary)) != NULL) {					
		
		if(process->suspended) {			
			
			if(kill(process->pid, SIGCONT) == -1) {

				printf("ERROR: Unable to signal child process: %s\n", process->name);
				exit(-1);
			}						
			
		} else {		
			
			free_index = find_free_block(process);
			
			if(free_index == -1) {
                
				//printf("Unable to find space for %s\n", process->name);
				
				push(&secondary, process);
				continue;
			}
			
			process->address = free_index;
			
			for(int i = 0; i < process->memory; i++) {

				avail_mem[process->address + i] = 1;
			}		
					
			pid = fork();
			
            //child
			if(pid == 0) {				
				
				execl("./process", "./process", NULL);
				
				printf("ERROR: Unable to call exec: %s\n", process->name);
				exit(-1);
				
			} else if(pid == -1) {
				printf("ERROR: Unable to create child process\n");
				exit(-1);
				
            //parent  
			} else {
							
				process->pid = pid;

				printf("%s, %d, %d, %d, %d\n", process->name, process->priority, process->pid, process->memory, process->runtime);				
			}
		}
		
		sleep(1);
		process->runtime--;		
		
		if(process->runtime == 0) {
			
			if(kill(process->pid, SIGINT) == -1) {

				printf("ERROR: Unable to signal child process: %s\n", process->name);
				exit(-1);
			}
			
			if(waitpid(process->pid, &status, 0) != process->pid) {

				printf("ERROR: Unable to wait for child process: %s\n", process->name);
				exit(-1);
			}
						
			for(int i = 0; i < process->memory; i++) {

				avail_mem[process->address + i] = 0;
			}	
		
			free(process);							
		} else {
		
			if(kill(process->pid, SIGTSTP) == -1) {

				printf("ERROR: Unable to signal child process: %s\n", process->name);
				exit(-1);
			}
			
			if(waitpid(process->pid, &status, WUNTRACED) != process->pid) {

				printf("ERROR: Unable to wait for child process: %s\n", process->name);
				exit(-1);
			}
						
			process->suspended = true;
			push(&secondary, process);
		}		
	}
	
	return 0;
}
