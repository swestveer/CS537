#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include "Queue.h"


typedef struct Arguments {
	struct Queue *Munch1Q;
	struct Queue *Munch2Q;
	struct Queue *WriterQ;
} Arguments;


void ReaderProcess(void *arg);
void Munch1Process(void *arg);
void Munch2Process(void *arg);
void WriterProcess(void *arg);

int main(){

	struct Arguments Arg;
	Arg.Munch1Q = CreateStringQueue(QUEUESIZE);
	Arg.Munch2Q = CreateStringQueue(QUEUESIZE);
	Arg.WriterQ = CreateStringQueue(QUEUESIZE);
  
  printf("Munch1 Q %p\n", Arg.Munch1Q);
printf("Munch2 Q %p\n", Arg.Munch2Q);
printf("Writer Q %p\n", Arg.WriterQ);

	pthread_t Reader, Munch1, Munch2, Writer;

	void *dummy = (void*)&Arg;


	pthread_create(&Reader, NULL, (void*)&ReaderProcess, dummy);
	pthread_create(&Munch1, NULL, (void*)&Munch1Process, dummy);
	pthread_create(&Munch2, NULL, (void*)&Munch2Process, dummy);
	pthread_create(&Writer, NULL, (void*)&WriterProcess, dummy);

	pthread_join(Reader, NULL);
	pthread_join(Munch1, NULL);
	pthread_join(Munch2, NULL);
	pthread_join(Writer, NULL);

	return 0;
}

void ReaderProcess(void *arg){

	struct Arguments *Arg = (struct Arguments*)arg;
	char *string = (char*)malloc(BUFFERSIZE*sizeof(char));
	char c;
  int pos = 0;

	while (1){ 
 c  = fgetc(stdin);
if (c == EOF){
	printf("End of FILE~~~~\n");
		break;
}
     // printf("c pos %c %d\n", c, pos);     
	  	if (c == '\n' || '\0'){
					string[pos] = c; 
	  			pos++;
     	    string[pos] = '\0'; 
          pos = 0;
          EnqueueString(Arg->Munch1Q,string);
					printf("Reader -> Munch1 Enqueue: %s\n",string);
					//printf("%s",string);
			}
      else{
    		  string[pos] = c;
    		  pos++;
			}
     
  }
	/*while (fgets(string, BUFFERSIZE,stdin) != NULL){
		//printf("input: %s\n", string);
		EnqueueString(Arg->Munch1Q,string);
		printf("Reader -> Munch1 Enqueue\n");
		printf("%s",string);
	}*/

	char kill = 'k';
	EnqueueString(Arg->Munch1Q,&kill);
	pthread_exit(NULL);
}

void Munch1Process(void *arg){

	struct Arguments *Arg = (struct Arguments*)arg;


	while (1){
		
		char *string = DequeueString(Arg->Munch1Q);
	//	printf("Munch1 Dequeue: %s\n" ,string);
		//printf("%s",string);
		char *p = string;

		if (*string == 'k'){
			EnqueueString(Arg->Munch2Q,string);
			//printf("Munch1 Q\n");
			//PrintQueueStats(Arg->Munch1Q);
			pthread_exit(NULL);
		}

		while (*p != '\0'){
			if (*p == 32){
				*p = '*';
			}
			p++;
		}
		EnqueueString(Arg->Munch2Q,string);
	//	printf("Munch1 -> Munch2 Enqueue: %s\n",string);
	//	printf("%s",string);
	}
}

void Munch2Process(void *arg){

	struct Arguments *Arg = (struct Arguments*)arg;

	while (1){

		char *string = DequeueString(Arg->Munch2Q);
	//	printf("Munch2 Dequeue\n");
	//	printf("%s",string);
	
		char *p = string; 
	
		if (*string == 'k'){
			EnqueueString(Arg->WriterQ,string);
			//printf("Munch2 Q\n");
			//PrintQueueStats(Arg->Munch2Q);
			pthread_exit(NULL);
		}

		while (*p != '\0'){
			if (islower(*p)){
				*p = toupper(*p);
			}
			p++;
		}
		EnqueueString(Arg->WriterQ,string);
		//printf("Munch2 -> Writer Enqueue: %s\n",string);
		//printf("%s",string);
	}
}

void WriterProcess(void *arg){

	struct Arguments *Arg = (struct Arguments*)arg;
	while (1){
		char *string = DequeueString(Arg->WriterQ);
	//	printf("Writer Dequeue\n");
		printf("Writer string %s",string);
		if (*string == 'k'){
			//printf("Writer Q\n");
			PrintQueueStats(Arg->WriterQ);
			pthread_exit(NULL);
		}
		//printf("%s\n",string);
	}
}


