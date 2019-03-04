///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #3
//
// Authors:          Shawn Westveer westveer@wisc.edu
//					Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include <unistd.h>
#include <sys/wait.h>
void proc_creation_prog_exe(struct Node **tar, int nTar, char **input){ 

	pid_t child, childStat; 
	int status;

	for (int i=0; i<nTar; i++){
		for (int j = 0; j<tar[i]->nCmd; j++){

			int startInd = tar[i]->cmd[j];
			int col = 0;

		
			// child process
			if((child = fork()) == 0){
				execvp(input[startInd+col], &input[startInd+col]);
				printf("execvp failed on command: %s\n", input[startInd+col]);
				exit(1);
			}
			// parent process
			else{
				if (child < 0){
					printf("fork failed\n");
					exit(1);
				}
				// wait for child process
				status = wait(&childStat);
				// check for child status
				if (childStat != 0){
					printf("Child %ld exited command %s with status %d\n",(long)status, input[startInd+col],childStat);
					exit(1);
				}
			}	
		}
	}
}	

