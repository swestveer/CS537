///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #1
//
// Authors:          Shawn Westveer westveer@wisc.edu
//									 Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#ifndef __prodcomm__Queue__
#define __prodcomm__Queue__

#include <pthread.h>

const int QUEUESIZE;
const int BUFFERSIZE;

typedef struct Queue{
	int enqueueCount;
	int dequeueCount;
	int enqueueBlockCount;
	int dequeueBlockCount;
	pthread_mutex_t mutex;
	pthread_cond_t full, empty;
	int first, last;
	char **str;

} Queue;

Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char *DequeueString(Queue *q);
void PrintQueueStats(Queue *q);

#endif /* defined(__prodcomm__Queue__) */
