///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #1
//
// Authors:          Shawn Westveer westveer@wisc.edu
//		     Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h" 
#include "getinfo.h"
#include "printProc.h"


int main(int argc, char *argv[]){

	uid_t uid = getuid();
	int flags[5], pid, nProc, pCount;

	nProc = 1000; // number of process with info stored 

	struct Process *procs = (struct Process*)malloc(nProc*sizeof(struct Process));	

	// parse command line arguement and determine print options
	parse(argc, argv, flags, &pid);

	// obtain process info for specified processes
	getinfo(procs, nProc, uid, pid, &pCount);

	// print process base on flags
	printProc(flags, procs, pCount);

	free(procs);

	return 0;
}
