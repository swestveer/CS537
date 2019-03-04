///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #3
//
// Authors:          Shawn Westveer westveer@wisc.edu
//					Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "build_spec_graph.h"
#include <sys/stat.h>
#include <time.h>

int traverse(struct Node *head, char **input, int *nTar, struct Node **tar);

struct Node **build_spec_repr(struct Node *head, char **input, int *nTar, char *Target){

	// allocate space to store pointers to targets that need recompiling
	struct Node **tar = (struct Node**)calloc(head->nChild,sizeof(struct Node*));	
	
	// find the position of Target in the children of the head
	struct Node *tarNode = tarPos(head,Target); 

	// initialized hasCalled
	for (int i=0; i<head->nChild; i++){
		head->child[i]->hasCalled = 0;
	}
	// start recursion at root
	traverse(tarNode,input,nTar,tar);

	return tar;
}

int traverse(struct Node *head, char **input, int *nTar, struct Node **tar){

	// mark node as called
	head->hasCalled = 1;

	int recomp = 0;
	int childRecomp = 0;
	struct stat f1, f2; // pointer to stat struct
	
	// if target exists
	if (fopen(head->name,"r") != NULL){
		stat(head->name,&f1);
		for (int i=0; i<head->nChild; i++){
			// if child exists 
			if (fopen(head->child[i]->name,"r") != NULL){
				stat(head->child[i]->name,&f2);
				if (difftime(f1.st_mtime, f2.st_mtime) < 0)
					recomp = 1;
			}
			// if child does not exist and is not a target
			else{
				if (!head->child[i]->isTar){
					printf("%s needed by %s does not exist\n", head->child[i]->name, head->name);
					exit(1);
				}
				recomp = 1;
			}
		}

	}	
	else{
		recomp = 1;	
	}
	
	// if target has no children
	if (head->nChild == 0)
		recomp = 1;
	
	// traverse
	for (int i=0; i<head->nChild; i++){
		// if the child is also a target 
		if (head->child[i]->isTar){
			if (!head->child[i]->hasCalled){
				childRecomp = traverse(head->child[i],input,nTar,tar);
				// if child needs recompiling
				if (childRecomp)
					recomp = 1;
			}
		}
	}
	if (recomp){
		tar[*nTar] = head;
		(*nTar)++;
	}
	return recomp; 
}
