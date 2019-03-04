#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "Queue.h"


const int QUEUESIZE=200;
const int BUFFERSIZE=1024;

int ModIncr(int v){
	return ((v+1) % QUEUESIZE);
}

struct Queue *CreateStringQueue(int size){

	struct Queue *q = (struct Queue*)malloc(6*sizeof(int)
			+sizeof(char**)
			+sizeof(pthread_mutex_t)
			+2*sizeof(pthread_cond_t));
	q->str = (char**)malloc(size*sizeof(char*));
	for (int i=0; i<size; i++){
		q->str[i] = (char*)calloc(BUFFERSIZE,sizeof(char*));
	}
	q->enqueueCount = 0;
	q->dequeueCount = 0;
	q->enqueueBlockCount = 0;
	q->dequeueBlockCount = 0;
	q->first = 0;
	q->last = 0;
	return q;
}


void EnqueueString(struct Queue *q, char *string){

	pthread_mutex_lock(&(q->mutex));

	while (ModIncr(q->last) == q->first) {
		// block
		pthread_cond_wait(&(q->full),&(q->mutex));
		q->enqueueBlockCount++;
	}
	q->enqueueCount++;
	q->str[q->last] = string;
	//printf("Enqueue str[i]\n");
	printf("enqueue %p %d %d %s\n",q,q->last,ModIncr(q->last),string);
	q->last = ModIncr(q->last);

	pthread_cond_signal(&(q->empty));
	pthread_mutex_unlock(&(q->mutex));

}

char *DequeueString(struct Queue *q){

	pthread_mutex_lock(&(q->mutex));
	while (q->first == q->last){
		// block

		pthread_cond_wait(&(q->empty),&(q->mutex));
		q->dequeueBlockCount++;

	}

//printf("%d\n",q->first);
char *removed = q->str[q->first];
	//char *removed = q->str[q->first];
	//printf("Dequeue str[i]\n");
	printf("dequeue %p %d %d %s\n",q, q->first,q->last, q->str[q->first]);
	q->first = ModIncr(q->first); 
	q->dequeueCount++;
	
	pthread_cond_signal(&(q->full));

	pthread_mutex_unlock(&(q->mutex));

	return removed;
}

void PrintQueueStats(struct Queue *q){

	pthread_mutex_lock(&(q->mutex));

	printf("enqueueCount: %d\n", q->enqueueCount);
	printf("dequeueCount: %d\n", q->dequeueCount);
	printf("enqueueBlockCount: %d\n", q->enqueueBlockCount);
	printf("dequeueBlockCount: %d\n", q->dequeueBlockCount);

	pthread_mutex_unlock(&(q->mutex));
}
