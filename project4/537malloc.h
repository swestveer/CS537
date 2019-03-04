///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #4
//
// Authors:          Shawn Westveer westveer@wisc.edu
//		     Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#ifndef __537malloc__537malloc__
#define __537malloc__537malloc__

#include <stdlib.h>
#include <stdio.h>
#include "node.h"

void *malloc537(size_t size);
void free537(void *ptr);
void *realloc537(void *ptr, size_t size);
void memcheck537(void *ptr, size_t size);

#endif /* defined(__537malloc__537malloc__) */

