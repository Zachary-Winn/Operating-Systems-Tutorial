#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 256
#define NUM_LINES 10

typedef struct proc {
	char	name[BUFFER_LEN];
	int priority;
	int pid;
	int runtime;
} proc;

typedef struct queue {
	proc *process;
	struct queue *next;
} queue;


//Our linked list of processes
queue *linked_list = NULL;


//Adds the process to our queue
void push(proc *process) {
	queue *new_node, *cur_node;
	
	//Allocate space for the new node
	new_node = (queue *) calloc(1, sizeof(queue));
		
	//Configure the node
	new_node->process = process;
	new_node->next = NULL;
	
	//Check if assigning the head
	if(linked_list == NULL) {
		linked_list = new_node;
		return;
	} 
	
	//Start at the head
	cur_node = linked_list;
	
	//Find last node
	while(cur_node->next != NULL) {
		cur_node = cur_node->next;
	}
	
	//Add the new node
	cur_node->next = new_node;	
}

//Removes the first entry from our queue and returns it's process
proc *pop() {
	queue *cur_node;
	proc *process;
	
	//Check if empty queue
	if(linked_list == NULL) {
		return NULL;
	}
	
	//Get the current head
	cur_node = linked_list;
	
	//Get the process info
	process = cur_node->process;
	
	//Update the head
	linked_list = cur_node->next;
	
	//Free the memory
	free(cur_node);
	
	//Return the process info of the node removed
	return process;
}

//Removes the entry with the specified name and returns it's process
proc *delete_name(char *name) {
	queue *cur_node, *removal_node;
	proc *process;
	
	//Check if empty queue
	if(linked_list == NULL) {
		return NULL;
	}
	
	//Get the current head
	cur_node = linked_list;
	
	//Get the process of the head
	process = cur_node->process;
	
	//Check if process being removed is at the head
	if(!strcmp(process->name, name)) {
		
		//Set the new head
		linked_list = cur_node->next;
		
		//Free the memory of the removal node
		free(cur_node);
			
		//Return the process that was just removed
		return process;
	}
	
	//Go through the queue
	while(cur_node->next != NULL) {
		
		//Get the process info
		process = cur_node->next->process;
		
		//Check if name was a match
		if(!strcmp(process->name, name)) {
			
			//Get the node being removed
			removal_node = cur_node->next;
						
			//Set the new next node
			cur_node->next = removal_node->next;
			
			//Free the memory of the removal node
			free(removal_node);
			
			//Return the process that was just removed
			return process;
		}
		
		//Get the next node
		cur_node = cur_node->next;
	}
	
	//Return process not found
	return NULL;
}

//Removes the entry with the specified process id and returns it's process
proc *delete_pid(int pid) {
	queue *cur_node, *removal_node;
	proc *process;
	
	//Check if empty queue
	if(linked_list == NULL) {
		return NULL;
	}
	
	//Get the current head
	cur_node = linked_list;
	
	//Get the process of the head
	process = cur_node->process;
	
	//Check if process being removed is at the head
	if(process->pid == pid) {
		
		//Set the new head
		linked_list = cur_node->next;
		
		//Free the memory of the removal node
		free(cur_node);
			
		//Return the process that was just removed
		return process;
	}
	
	//Go through the queue
	while(cur_node->next != NULL) {
		
		//Get the process info
		process = cur_node->next->process;
		
		//Check if name was a match
		if(process->pid == pid) {
			
			//Get the node being removed
			removal_node = cur_node->next;
						
			//Set the new next node
			cur_node->next = removal_node->next;
			
			//Free the memory of the removal node
			free(removal_node);
			
			//Return the process that was just removed
			return process;
		}
		
		//Get the next node
		cur_node = cur_node->next;
	}
	
	//Return process not found
	return NULL;
}

int main(void) {
	FILE *fp;
	proc *process;
	queue *cur_node;
	
	//Open processes file
	fp = fopen("processes.txt", "r");
	
	//Check if open failed
	if(fp == NULL) {
		printf("ERROR: Unable to open processes.txt\n");
		return -1;
	}
	
	//Read the lines
	for(int i = 0; i < NUM_LINES; i++) {
		
		//Allocate space for the process info
		process = (proc *) calloc(1, sizeof(proc));
				
		//Read the data into the struct
		fscanf(fp, "%[^,], %d, %d, %d\n", process->name, &process->priority, &process->pid, &process->runtime);
		
		//Add the process to our queue
		push(process);
	}
	
	//Remove the emacs process
	process = delete_name("emacs");
	
	//Check if error
	if(process == NULL) {
		printf("ERROR: emacs process not found\n");
		return -1;
	}
	
	//Display the removed process info
	printf("Removed by name: %s, %d, %d, %d\n", process->name, process->priority, process->pid, process->runtime);
	
	//Remove the process with id 12235
	process = delete_pid(12235);
	
	//Check if error
	if(process == NULL) {
		printf("ERROR: process with id 12235 not found\n");
		return -1;
	}
	
	//Display the removed process info
	printf("Removed by id: %s, %d, %d, %d\n", process->name, process->priority, process->pid, process->runtime);
	
	//Remove all the processes in the queue
	while((process = pop()) != NULL) {
		printf("%s, %d, %d, %d\n", process->name, process->priority, process->pid, process->runtime);
	}
	
	//Close the file
	fclose(fp);
	
	return 0;
}
