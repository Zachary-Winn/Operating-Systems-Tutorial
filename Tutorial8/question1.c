#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int treesize =0;


struct proc{
	char parent[256];
	char name[256];
	int priority;
	int memory;
};

struct proc_tree{
	struct proc node;
}*head,*current_parent;


int addnode(char parentName[256], char nodeName[256],int nodePriority, int nodeMemory){
	struct proc_tree *newNode;
	newNode = (struct proc_tree *) malloc(sizeof(struct proc_tree));
	if (treesize == 0){
		//if tree empty, make tree root
		strcpy(newNode->node.parent,parentName); 
		strcpy(newNode->node.name,nodeName); 
		newNode->node.priority = nodePriority;
		newNode->node.memory = nodeMemory;
	}
	else{
		char parentName[256];
		//find the proper place for the next place


		strcpy(newNode->node.parent,parentName); 
		strcpy(newNode->node.name,nodeName); 
		newNode->node.priority = nodePriority;
		newNode->node.memory = nodeMemory;
	}



	++treesize;
	return 0;
}

int main(void){

	FILE *fp;
	const char delim[2] = ",";
	char buffer[256], *token, tempParent[256],tempName[256];
	int c, buffcount=0, tempPriority, tempMemory;
	fp = fopen("processes_tree.txt","r");

	//for each line
	for(int i = 0; i<7;i++){

		//fill buffer with one line
		while((c = fgetc(fp))!= '\n'){
			buffer[buffcount++] =c; 
		}

		buffcount = 0; //reset buffer counter
		
		//split line based on comma delimiter

		token = strtok(buffer,delim);
        
		for(int j = 0;j<4;j++){
			switch(j){
				case 0:strcpy(tempParent,token);break;
				case 1:strcpy(tempName,token);break;
				case 2:tempPriority = atoi(token);break;
				case 3:tempMemory = atoi(token);break;
			}
            token = strtok(NULL,delim);
		}

		addnode(tempParent,tempName,tempPriority,tempMemory);

        //clean buffer
        for(int b =0; b<256;b++){
            buffer[b] = '\0';
        }
    }

	fclose(fp);

	return 0;
}
