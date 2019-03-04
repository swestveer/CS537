///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #3
//
// Authors:          Shawn Westveer westveer@wisc.edu
//					Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#ifndef __537make__build_spec_graph__
#define __537make__build_spec_graph__

#include <stdlib.h>
#include <stdio.h>
#include "text_parsing.h"
#include "node.h"

struct Node *build_spec_graph(char **input, char *Target,int nRow); 
struct Node *tarPos(struct Node *head, char *tarName);

#endif /* defined(__537make__build_spec_graph__) */
