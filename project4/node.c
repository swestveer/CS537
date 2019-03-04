///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #4
//
// Authors:          Shawn Westveer westveer@wisc.edu
//		     Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include "node.h"

struct node *root = NULL;

// insert 
// credit: GeeksforGeeks - Red-Black Tree | Set 2 (Insert), improved by ChiragAcharya
// credit: GeeksforGeeks - C Program for Red Black Tree Insertion, by Mohsin Mohammaad 
struct node *createNode(void *addr, size_t size); 
struct node *RBinsertNode(void *addr, size_t size);
struct node *BSTinsertNode(struct node *head, void *addr, size_t size, struct node **ptr);
void fixTree(struct node *ptr);
void rotateLeft(struct node *parent_pt);
void rotateRight(struct node *parent_pt);
void swapColor(int *x, int *y); 

// delete
// credit: GeeksforGeeks - Red-Black Tree | Set 3 (Delete), 
// improved by shwetanknaveen, BhanuPratapsSinghRathore
void RBdeleteNode(struct node *v);
struct node *BSTreplace(struct node *v);
struct node *successor(struct node *x);
void fixDoubleBlack(struct node *x);
struct node *Uncle(struct node *x);
struct node *Sibling(struct node *x);
int isOnLeft(struct node *x);
void moveDown(struct node *x, struct node *nParent);
void DrightRotate(struct node *x);
void DleftRotate(struct node *x);
int hasRedChild(struct node *x); 

// printing tree
void printLevelOrder(struct node *head);
void printGivenLevel(struct node *head, int level); 
int height(struct node *node); 

// finding overlaps in tree
struct node **overlap(void *addr, size_t size, int *nOvers); 
struct node *over(void *ptr);

//////////////////// Insertion functions ////////////////////////////////
// allocate memory for new node and iniitialize to default
struct node *createNode(void *addr, size_t size){

	struct node *new = (struct node*)calloc(1,sizeof(node));
	// check for system call
	if (new == NULL){
		fprintf(stderr, "calloc failed \n"); 
		exit(-1); 
	}
	new->ptr = addr;
	new->size = size;
	new->freed = 0;
	new->left = NULL;
	new->right = NULL;
	// 1- red, 0-black
	new->color = 1; 
	return new;
}
// fix violation after insertion
void fixTree(struct node *pt){

	struct node *parent_pt = NULL;
	struct node *grand_parent_pt = NULL;

	while ((pt != root) && (pt->color != 0) && (pt->parent->color == 1)){

		parent_pt = pt->parent;
		grand_parent_pt = pt->parent->parent;

		// case: A
		// Parent of pt is left child of grand-parent of pt
		if (parent_pt == grand_parent_pt->left){

			struct node *uncle_pt = grand_parent_pt->right;
			// case: 1
			// Uncle is also red
			// Recoloring
			if ((uncle_pt != NULL) && (uncle_pt->color == 1)){
				grand_parent_pt->color = 1;
				parent_pt->color = 0; 
				uncle_pt->color = 0; 
				pt = grand_parent_pt; 
			}
			else{
				// case: 2
				// pt is right child of its parent
				// left-rotation
				if (pt == parent_pt->right){
					rotateLeft(parent_pt);
					pt = parent_pt; 
					parent_pt = pt->parent;
				}
				// case: 3
				// pt is left child of its parent
				// right-rotation
				rotateRight(grand_parent_pt);
				swapColor(&(parent_pt->color),&(grand_parent_pt->color));
				pt = parent_pt; 
			}
		}

		// case : B
		// parent of pt is right child of grand-parent of pt
		else{
			struct node *uncle_pt = grand_parent_pt->left; 
			// case : 1
			// uncle is red
			// recolor
			if ((uncle_pt != NULL) && (uncle_pt->color == 1)){
				grand_parent_pt->color = 1; 
				parent_pt->color = 0; 
				uncle_pt->color = 0; 
				pt = grand_parent_pt; 
			}
			else{
				// case : 2
				// pt is left child of its parent
				// right-rotation
				if (pt == parent_pt->left){
					rotateRight(parent_pt);
					pt = parent_pt; 
					parent_pt = pt->parent; 
				}
				// case : 3
				// pt is right child of its parent
				// left rotation
				rotateLeft(grand_parent_pt);
				swapColor(&(parent_pt->color), &(grand_parent_pt->color));
				pt = parent_pt; 
			}
		}
	}
	root->color = 0; 
}
// swap 
void swapColor(int *x, int *y){
	int dum = *x; 
	*x = *y; 
	*y = dum;
}
// used in fix tree to restored Red-Black tree invariants
void rotateLeft(struct node *pt){

	struct node *pt_right = pt->right; 

	pt->right = pt_right->left; 

	if (pt->right != NULL)
		pt->right->parent = pt; 
	pt_right->parent = pt->parent; 

	if (pt->parent == NULL)
		root = pt_right;
	// left-left rotation
	else if (pt == pt->parent->left)
		pt->parent->left = pt_right;
	// left-right rotation
	else
		pt->parent->right = pt_right;
	pt_right->left = pt; 
	pt->parent = pt_right; 
}
// used in fix tree to restored Red-Black tree invariants
void rotateRight(struct node *pt){

	struct node *pt_left = pt->left; 
	pt->left = pt_left->right; 
	if (pt->left != NULL)
		pt->left->parent = pt; 
	pt_left->parent = pt->parent; 
	if (pt->parent == NULL)
		root = pt_left; 
	// right-left rotation
	else if (pt == pt->parent->left)
		pt->parent->left = pt_left; 
	// right-right rotation
	else
		pt->parent->right = pt_left; 
	pt_left->right = pt;
	pt->parent = pt_left;
}
// Red-Black tree insertion method
struct node *RBinsertNode(void *addr, size_t size){

	struct node *new = NULL; 
	BSTinsertNode(root, addr, size, &new);

	// fix RB tree violations
	fixTree(new); 

	return new;
}
// Binary search tree insertion
struct node *BSTinsertNode(struct node *head, void *addr, size_t size, struct node **ptr){

	// if tree is empty
	if (head == NULL){
		struct node *newNode = createNode(addr, size);
		*ptr = newNode; 
		// if the entire tree is empty
		if (root == NULL){
			root = newNode;
			root->color = 0; 
		}
		return newNode; 
	}
	// otherwise traverse down the tree
	if (addr < head->ptr){
	//if (size < head->size){
		head->left = BSTinsertNode(head->left, addr, size, ptr);
		head->left->parent = head;
	}
	else if (addr > head->ptr){
	//else if (size > head->size){
		head->right = BSTinsertNode(head->right, addr, size, ptr);	
		head->right->parent = head;
	}
	return head;
}	
//////////////////// Printing functions ////////////////////////////////
// find height of RB tree
int height(struct node *node){

	if (node == NULL)
		return 0;
	int lheight = height(node->left);
	int rheight = height(node->right);

	if (lheight > rheight)
		return lheight + 1;
	return rheight + 1;

}
// print nodes level by level 
void printLevelOrder(struct node *head){

	int h = height(head);
	int i;
	for (i=1; i<=h; i++){
		printGivenLevel(head, i);
		printf("\n");
	}
}
// print nodes in given level
void printGivenLevel(struct node *head, int level){

	if (head == NULL)
		return; 
	if (level == 1){
		printf("(%p, %d, %ld) ", head->ptr, head->color, head->size);
	}
	else if (level > 1){
		printGivenLevel(head->left, level-1);
		printGivenLevel(head->right, level-1);
	}
}
//////////////////// Deletion functions ////////////////////////////////
// Red Black tree deletion
void RBdeleteNode(struct node *v){
	struct node *u = BSTreplace(v);

	// 1 when u and v are both black 
	int uvBlack = 0; 
	if (((u == NULL) || (u->color == 0)) && (v->color == 0)){
	       uvBlack = 1;
	}
	struct node *par = v->parent; 	

	// if v is a leaf
	if (u == NULL){
		// if v is root 
		if (v == root){
			root = NULL;	
		}
		else{
			// if both are black
			// v is leaf, fix double black at v
			if (uvBlack){
				fixDoubleBlack(v);
			}
			else {
				// u or v is red
				if (Sibling(v) != NULL){
					// sibling is not null, 
					// make it red
					Sibling(v)->color = 1;
				}
			}
			// delete v from tree
			if (isOnLeft(v)){
				v->parent->left = NULL;
			}
			else{
				v->parent->right = NULL;
			}
		}
		free(v);
		return;
	}
	// v has 1 child
	if ((v->left == NULL) || (v->right == NULL)){
		if (v == root){
			// v is root, assign value u to v 
			// and delete u
			v->ptr = u->ptr; 
			v->size = u->size;
			v->freed = u->freed;
			v->left = NULL;
			v->right = NULL;
			free(u);
		}
		else{
			// detach v from tree and move u up
			if (isOnLeft(v)){
				v->parent->left = u;
			}
			else{
				v->parent->right = u;
			}
			free(v);
			u->parent = par;
			if (uvBlack){
				// u and v both black,
				// fix double black
				fixDoubleBlack(u);
			}
			else{
				u->color = 0;
			}
		}
		return;
	}

	// v has 2 children, swap values with successor and recurse
	size_t tsize = u->size; 
	void *tptr = u->ptr;
	int tfreed = u->freed;
	u->size = v->size;
	u->ptr = v->ptr; 
	u->freed = v->freed; 
	v->size = tsize; 
	v->ptr = tptr; 
	v->freed = tfreed; 
	RBdeleteNode(u);	
}
// find node to replace to-be-deleted node
struct node *BSTreplace(struct node *v){

	// when node has 2 children
	if ((v->left != NULL) && (v->right != NULL))
	       return successor(v->right);

	// when leaf
	if ((v->left == NULL) && (v->right == NULL))
		return NULL;

	// when single child
	if (v->left != NULL)
		return v->left; 
	else
		return v->right;
}
// find successor 
struct node *successor(struct node *x){

	struct node *temp = x; 

	while (temp->left != NULL)
		temp = temp->left;
	return temp;
}
// fix Red-Black tree double black violation
void fixDoubleBlack(struct node *x){
	// reached root
	if (x == root)
		return;
	struct node *sib = Sibling(x);
	struct node *par = x->parent;

	// no sibling, double black pushed up
	if (sib == NULL){
		fixDoubleBlack(par);
	}
	else{
		// if sibling is red
		if (sib->color == 1){
			x->parent->color = 1;
			sib->color = 0;
			if (isOnLeft(sib)){
				DrightRotate(x->parent);
			}
			else{
				DleftRotate(x->parent);
			}
			fixDoubleBlack(x);
		}
		// sibling is black
		if (hasRedChild(sib)){
			if ((sib->left != NULL) && (sib->left->color == 1)){
				if (isOnLeft(sib)){
					// left left
					sib->left->color = sib->color; 
					sib->color = x->parent->color; 
					DrightRotate(x->parent);
				}
				else{
					// right left
					sib->left->color = x->parent->color; 
					DrightRotate(sib);
					DleftRotate(x->parent);
				}
			}
			else{
				if (isOnLeft(sib)){
					// left right
					sib->right->color = x->parent->color;
					DleftRotate(sib);
					DrightRotate(x->parent);
				}
				else{
					// right right
					sib->right->color = sib->color;
					sib->color = x->parent->color;
					DleftRotate(x->parent);
				}
			}
			x->parent->color = 0; 
		}
		else{
			// sibling has 2 black children
			sib->color = 1;
			if (x->parent->color == 0){
				fixDoubleBlack(x->parent);
			}
			else{
				x->parent->color = 0; 
			}
		}
	}

}
// find uncle
struct node *Uncle(struct node *x){
	// no uncle if no parent or grandparent
	if ((x->parent == NULL) || (x->parent->parent == NULL))
		return NULL;
	// if uncle is on the right
	if (isOnLeft(x->parent))
		return x->parent->parent->right;
	return x->parent->parent->left;
}
// find sibling
struct node *Sibling(struct node *x){

	// if no parent, then no sibling
	if (x->parent == NULL)
		return NULL;
	if (isOnLeft(x))
		return x->parent->right;
	return x->parent->left; 

}
// check if node is a left child
int isOnLeft(struct node *x){
	if (x->parent->left == x)
		return 1;
	return 0;
}
// move nParent in between x and x->parent
void moveDown(struct node *x, struct node *nParent){
	if (x->parent != NULL){
		if (isOnLeft(x)){
			x->parent->left = nParent; 
		}
		else{
			x->parent->right = nParent; 
		}
	}
	nParent->parent = x->parent;
	x->parent = nParent; 
}
// check if node has red child
int hasRedChild(struct node *x){

	if ((x->left != NULL) && (x->left->color == 1))
		return 1;
	if ((x->right != NULL) && (x->right->color == 1))
		return 1;
	return 0;
}
// rotation used in fixDoubleBlack
void DrightRotate(struct node *x){
	// new parent will be node's left child
	struct node *nParent = x->left;

	// update root if current node is root
	if (x == root)
		root = nParent;

	moveDown(x,nParent);
	// connect x with new parent's left element
	x->left = nParent->right; 
	// if it is not null
	if (nParent->right != NULL)
		nParent->right->parent = x;
	// connect new parent with x
	nParent->right = x;
}
// rotation used in fixDoubleBlack
void DleftRotate(struct node *x){
	// new parent will be node's right child
	struct node *nParent = x->right;

	// update root if current node is root
	if (x == root)
		root = nParent;

	moveDown(x,nParent);
	// connect x with new parent's left element
	x->right = nParent->left; 
	// if it is not null
	if (nParent->left != NULL)
		nParent->left->parent = x;
	// connect new parent with x
	nParent->left = x;
}
// find overlap with the node to be inserted
struct node **overlap(void *addr, size_t size, int *nOvers){

	int maxOvers = 1024; 
	struct node **overlist = (struct node **)calloc(maxOvers, sizeof(struct node*));
	// check for system call
	if (overlist == NULL){
		fprintf(stderr, "calloc failed \n"); 
		exit(-1); 
	}

	void *nptr = addr; 
	struct node *head;  

	while(nptr < (addr+size)){

		head = root; 
	
		while (head != NULL){
			
			// check if there is an overlap with the current node
			if ((nptr >= head->ptr) && (nptr < (head->ptr + head->size))){
				// add to the overlist
				overlist[*nOvers] = head; 
				(*nOvers)++; 
				nptr = head->ptr+head->size; 
				if (*nOvers == maxOvers){
					maxOvers *= 2; 
					overlist = (struct node**)realloc(overlist, maxOvers*sizeof(struct node*)); 
					// check for system call
					if (overlist == NULL){
						fprintf(stderr, "realloc failed \n"); 
						exit(-1); 
					}
				}
				break;
			}
			if (nptr < head->ptr){
				head = head->left; 
			}
			else if (nptr > head->ptr){
				head = head->right; 
			}
		}
		nptr++; 
	}

	return overlist;
}
// find node that overlaps with ptr
struct node *over(void *ptr){

	struct node *head = root; ;  
	struct node *overlapNode = NULL; 

	// traverse tree to find the node that overlaps with ptr
	while (head != NULL){
		
		// check if there is an overlap with the current node
		if ((ptr >= head->ptr) && (ptr < (head->ptr + head->size))){
			overlapNode = head; 
			break;
		}
		if (ptr < head->ptr){
			head = head->left; 
		}
		else if (ptr > head->ptr){
			head = head->right; 
		}
	}
	return overlapNode; 
}
