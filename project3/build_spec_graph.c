///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #3
//
// Authors:          Shawn Westveer westveer@wisc.edu
//					Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "text_parsing.h"
#include <string.h>

const int BUFFERSIZE = 1024;

struct Node *initializeNode();
void DFSCycle(struct Node *head);
void printTree(struct Node *head, char **input);
void addToList(struct Node *head, struct Node *n);
void detectCycle(struct Node *head);
struct Node *tarPos(struct Node *head, char *tarName);

struct Node *build_spec_graph(char **input, char *Target, int nRow){ 

	// initialize head node
	struct Node *head = initializeNode();
	char *c = calloc(BUFFERSIZE,sizeof(char));
	strcpy(c,"root");
	head->name = c;	
	head->isRoot = 1;
		
	// find out how many targets
	int nTar = 0;
	for (int row = 0; row < nRow ; row++){
		if (input[row*BUFFERSIZE] == NULL)
			break;
		if (strcmp(input[BUFFERSIZE*row],"0") == 0){
			nTar++;
		}
	}	

	// add all targets as child of head
	head->nChild = nTar;
	if (nTar != 0){
		head->child = (struct Node **)calloc(nTar,sizeof(struct Node *)); 
	}
	else{
		printf("there are no targets\n");
	}
	nTar = 0;
	struct Node *n;
	for (int row = 0; row < nRow ; row++){
		if (input[row*BUFFERSIZE] == NULL)
			break;
		if (strcmp(input[BUFFERSIZE*row],"0") == 0){
			// initialize new node for target
			n = initializeNode();
			n->isTar = 1;	
			n->name = input[BUFFERSIZE*row+1];
			head->child[nTar] = n;
			nTar++;
		}
	}	
	// if target is not specified, set to the first target
	if (strcmp(Target,"") == 0){
		strcpy(Target,head->child[0]->name);
	}
	// else check if Target is in the makefile
	else if (tarPos(head,Target) == NULL){
		printf("specified target %s is not in the makefile\n", Target);	
		exit(0);
	}
	
	// keep track of number of nodes in tree
	char *tarName;
	struct Node *tarNode, *check; 
	int nC; 
	// add each input as a node in the tree 
	for (int row = 0; row < nRow; row++){
		if (input[row*BUFFERSIZE] == NULL)
			break;
		// if the input line is about target and dependencies
		// build the tree
		if (strcmp(input[BUFFERSIZE*row],"0") != 0)
			continue;
		// assign target name
		tarName = input[BUFFERSIZE*row+1];
		tarNode = tarPos(head,tarName);
		// find how many dependencies for target t
		nC = 0;
        	for (int col = 2; col < BUFFERSIZE; col++){
            		if (input[BUFFERSIZE*row+col] == NULL)
               			break;
			nC++;
		}
		// allocate array to store child nodes
		tarNode->nChild = nC;
		if (nC != 0){
			tarNode->child = (struct Node **)calloc(nC,sizeof(struct Node *)); 
		}
		// add child to list of dependencies
		nC = 0;
        	for (int col = 2; col < BUFFERSIZE; col++){
            		if (input[BUFFERSIZE*row+col] == NULL)
               			break;
			// check if the child is also a target
			check = tarPos(head,input[BUFFERSIZE*row+col]);	
			if (check != NULL){
				tarNode->child[nC] = check;
			}
			else{
				n = initializeNode();
				n->name = input[BUFFERSIZE*row+col];
				tarNode->child[nC] = n;
			}
			nC++;
		}       			
		// find how many command associate with target
		int line = row + 1;
		while (input[BUFFERSIZE*line] != NULL){
			if (strcmp(input[BUFFERSIZE*line],"0") == 0){
				break;	
			}
			if (strcmp(input[BUFFERSIZE*line],"1") == 0){
				tarNode->nCmd++;
			}
			line++;
		}
		// allocate space for command
		tarNode->cmd = (int*)calloc(tarNode->nCmd,sizeof(int)); 

		// add command to list of commands	
		int command = 0; 
		line = row + 1;
		while (input[BUFFERSIZE*line] != NULL){
			if (strcmp(input[BUFFERSIZE*line],"0") == 0){
				break;	
			}
			if (strcmp(input[BUFFERSIZE*line],"1") == 0){
				tarNode->cmd[command] = BUFFERSIZE*line+1;
			}
			line++;
			command++;
		}
	}
	// check for cycle and if dependencies are targets
	detectCycle(head);	

	return head;
}
struct Node *initializeNode(){

	struct Node *n = (struct Node*)malloc(5*sizeof(int)
			+sizeof(int*)+sizeof(char*)
			+sizeof(struct Node**));
	n->cmd = NULL;
	n->name = NULL;
	n->child = NULL;
	n->nChild = 0;
	n->hasCalled = 0;
	n->isTar = 0;
	n->isRoot = 0;
	n->nCmd = 0;
	return n;
}
struct Node *tarPos(struct Node *head, char *tarName){

	for (int i=0; i<head->nChild; i++){
		if (strcmp(head->child[i]->name,tarName) == 0){
			return head->child[i];
		}
	}
	return NULL;
}
void detectCycle(struct Node *head){
	
	// note that only targets will be part of a cycle
	// for each target, perform DFS
	for (int i=0; i<head->nChild; i++){
		// reset hasCalled
		for (int j=0; j<head->nChild; j++){
			head->child[j]->hasCalled = 0;
		}
		// perform DFS for target head->child[i]
		DFSCycle(head->child[i]);
	}	

}
void DFSCycle(struct Node *head){

	// mark node as visited
	head->hasCalled = 1;

	// if any of the children has been visited, there is a cycle
	for (int i=0; i<head->nChild; i++){
		if (head->child[i]->hasCalled){
			printf("cycle detected at %s\n", head->child[i]->name);
			exit(1);
		}
	}
	// otherwise, recurse
	for (int i=0; i<head->nChild; i++){
		if(head->child[i]->isTar){
			DFSCycle(head->child[i]);
		}
	}
	// mark node as visited
	head->hasCalled = 0;
}
