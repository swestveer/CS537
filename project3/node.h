///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #3
//
// Authors:          Shawn Westveer westveer@wisc.edu
//					Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#ifndef __537make__node__
#define __537make__node__

const int BUFFERSIZE;

typedef struct Node{
    struct Node **child;
    int *cmd;
    int nChild, isRoot, isTar, nCmd, hasCalled;
    char *name;
} Node;

#endif /* defined(__537make__node__) */
