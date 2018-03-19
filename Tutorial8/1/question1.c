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
	struct proc_tree *left;
	struct proc_tree *right;
}*head,*current_parent;

struct proc_tree * nodeFind(struct proc_tree *node, char parentName[256]){
	struct proc_tree * tempNode;
	

	
}

int addnode(char parentName[256], char nodeName[256],int nodePriority, int nodeMemory){
	struct proc_tree *newNode;
	newNode = (struct proc_tree *) malloc(sizeof(struct proc_tree));
	if (treesize == 0){
		printf("creating head\n");
		//if tree empty, make tree root
		strcpy(newNode->node.parent,parentName); 
		strcpy(newNode->node.name,nodeName); 
		newNode->node.priority = nodePriority;
		newNode->node.memory = nodeMemory;

		head = newNode;
	}
	else{
		//find the proper place for the next place

		printf("finding node\n");
		newNode = nodeFind(head,parentName);
		printf("newNode is: %s\n",newNode->node.name);
		strcpy(newNode->node.parent,parentName); 
		strcpy(newNode->node.name,nodeName); 
		newNode->node.priority = nodePriority;
		newNode->node.memory = nodeMemory;
	}

	newNode->left  = NULL;
	newNode->right = NULL;
	++treesize;
	printf("head value is: %s  treesize:%i\n\n",head->node.name,treesize);
	return 0;
}


void iterateAllNodes(struct proc_tree *currentNode){
	struct proc a = currentNode->node; 
	printf("parent: %s \nname: %s \npriority: %i \nmemory: %i\n",a.parent,a.name,a.priority,a.memory);
	if(currentNode->left!=NULL){
	iterateAllNodes(currentNode->left);
	}
	if(currentNode->right!=NULL){
	iterateAllNodes(currentNode->right);
	}
}



int main(void){
	printf("starting program\n");
	FILE *fp;
	const char delim[2] = ",";
	char buffer[256], *token, tempParent[256],tempName[256];
	int c, buffcount=0, tempPriority, tempMemory;
	printf("opening file\n");
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
	printf("closing file\n");
	fclose(fp);

	
	printf("\nprinting nodes\n");
	iterateAllNodes(head);
	return 0;
}
