///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #4
//
// Authors:          Shawn Westveer westveer@wisc.edu
//		     Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include "537malloc.h"
#include "node.h"

void *malloc537(size_t size){

	// give warning if size is 0
	if (size == 0){
		printf("warning: allocating memory block of size 0\n"); 
	}

	// allocate memory
	void *addr = malloc(size);

	// check system call
	if (addr == NULL){
		fprintf(stderr, "malloc failed \n");
		exit(-1); 
	}
	
	// find overlaps with tree 
	int nOvers = 0;
	struct node **overlist = overlap(addr, size, &nOvers);
	
	// delete overlapping nodes from tree
	for (int n = 0; n<nOvers; n++){
		RBdeleteNode(overlist[n]);
	}
	if (nOvers > 0)
		free(overlist); 
	
	// insert the new node into the tree
	RBinsertNode(addr,size);

	return addr;
}
void free537(void *ptr){
	
	// traverse tree to find the node that overlaps with ptr
	struct node *overlapNode = over(ptr); 

	// if overlap node is not found
	if (overlapNode == NULL){
		fprintf(stderr, "memory address %p has not been allocated\n", ptr);
		exit(-1);
	}
	// check if ptr is at the start of a node
	if (ptr != overlapNode->ptr){
		fprintf(stderr, "memory address %p is not at the start of memory block\n", ptr);
		exit(-1);
	}
	// check for double free
	if (overlapNode->freed){
		fprintf(stderr, "double free: memory address %p has already been freed\n", ptr);
		exit(-1);
	}
	// free memory block
	overlapNode->freed = 1;
	free(ptr); 	
}
void *realloc537(void *ptr, size_t size){

	// malloc if ptr is NULL
	if (ptr == NULL){
		return malloc537(size);
	}
	// free if size is zero
	if (size == 0){
		fprintf(stderr, "reallocating to size 0, freeing memory block\n"); 
		free537(ptr);
		return NULL; 
	}
	// check if ptr is a node
	struct node *overlapNode = over(ptr); 

	// if overlap node is not found
	if (overlapNode == NULL){
		fprintf(stderr, "memory address %p has not been allocated\n", ptr);
		exit(-1);
	}
	// if overlapping node has already been freed
	if (overlapNode->freed){
		fprintf(stderr, "memory address %p has already been freed\n", ptr);
		exit(-1);
	}
	// check if ptr is at the start of a node
	if (ptr != overlapNode->ptr){
		fprintf(stderr, "memory address %p is not at the start of memory block\n", ptr);
		exit(-1);
	}

	// malloc new addr
	void *addr = realloc(ptr, size); 

	// check system call
	if (addr == NULL){
		fprintf(stderr, "realloc failed \n");
		exit(-1); 
	}
	
	// delete old node
	RBdeleteNode(overlapNode);
	
	// find overlaps with tree 
	int nOvers = 0;
	struct node **overlist = overlap(addr, size, &nOvers);
	
	// delete overlapping nodes from tree
	for (int n = 0; n<nOvers; n++){
		RBdeleteNode(overlist[n]);
	}
	if (nOvers > 0)
		free(overlist); 
	
	// insert the new node into the tree
	RBinsertNode(addr,size);

	return addr;
}
void memcheck537(void *ptr, size_t size){

	// traverse tree to find the node that overlaps with ptr
	struct node *overlapNode = over(ptr); 

	// if overlap node is not found
	if (overlapNode == NULL){
		fprintf(stderr, "memory address %p has not been allocated\n", ptr);
		exit(-1);
	}
	// if overlapping node has already been freed
	if (overlapNode->freed){
		fprintf(stderr, "memory address %p has already been freed\n", ptr);
		exit(-1);
	}
	// if not the whole range has been allocated
	if ((ptr+size-1) > (overlapNode->ptr + overlapNode->size -1)){
		fprintf(stderr, "memory access %p to %p is out of bound\n", 
				overlapNode->ptr+overlapNode->size, ptr+size);
		exit(-1);
	}
}

