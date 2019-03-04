///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #1
//
// Authors:          Shawn Westveer westveer@wisc.edu
//									 Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include "getinfo.h"
#include "printProc.h"

void printProc(int *flags, struct Process *procs, int pCount){

	for(int i = 0; i < pCount; i++) {
		if (flags[0] == 1){
	  		printf("%10s: ", procs[i].pid);
		}
		if (flags[1] == 1){
	  		printf("%4s ", procs[i].state);
		}
		if (flags[2] == 1){
	  		printf("utime=%s ", procs[i].utime);
		}
		if (flags[3] == 1){
	  		printf("stime=%s ", procs[i].stime);
		}
		if (flags[4] == 1){
	  		printf("vmem=%s ", procs[i].vmem);
		}
		if (flags[5] == 1){
	  		printf("[%s]", procs[i].cmdline);
		}
		printf("\n");
	}  
}
