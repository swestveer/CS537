///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #4
//
// Authors:          Shawn Westveer westveer@wisc.edu
//		     Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#ifndef __537malloc__node__
#define __537malloc__node__

#include <stdlib.h>

typedef struct node {
	void *ptr; 
	struct node *left, *right, *parent;
	int color, freed; 
	size_t size;
} node;

extern struct node *root;

struct node *RBinsertNode(void *addr, size_t size);
struct node *BSTinsertNode(struct node *head, void *addr, size_t size, struct node **ptr);
struct node *searchNode(struct node *head, void *addr, size_t size);

void RBdeleteNode(struct node *v);
struct node *BSTreplace(struct node *v);
void fixDoubleBlack(struct node *x);

void printLevelOrder(struct node *head);
void printGivenLevel(struct node *head, int level); 
int height(struct node *node); 
struct node *over(void *ptr);
struct node **overlap(void *addr, size_t size, int *nOvers); 
#endif /* defined(__537malloc__node__) */
