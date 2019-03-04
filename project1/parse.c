///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #1
//
// Authors:          Shawn Westveer westveer@wisc.edu
//		     Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"

int checkOptarg(char *optarg);
void checkPid(char *optarg);

void parse(int argc, char *argv[], int *flags, int *pid){

	int c, n, option, ind; 

	*pid = -1; 
	// default flags
	flags[0] = 1; // p - process
	flags[1] = 0; // s - state
	flags[2] = 1; // U - utime - user time
	flags[3] = 0; // S - stime - system time
	flags[4] = 0; // v - size of virtual memory
	flags[5] = 1; // c - cmdline

	n = 1;
	while ((c = getopt(argc, argv, ":p:s::S::U::v::c::")) != -1){
		switch (c) {
		case 'p':
			flags[0] = 1; 
			// checks that pid is an integer
			checkPid(optarg);
			*pid = atoi(optarg);
			n++;
			break;
		case 's':
			option = checkOptarg(optarg);
			if (option == -1){
				printf("%c is not a valid option for s\n", *optarg);
				exit(0);
			}
			flags[1] = option; 
			n++;
			break;	
		case 'U':
			option = checkOptarg(optarg);
			if (option == -1){
				printf("%c is not a valid option for U\n", *optarg);
				exit(0);
			}
			flags[2] = option; 
			n++;
			break;	
		case 'S':
			option = checkOptarg(optarg);
			if (option == -1){
				printf("%c is not a valid option for S\n", *optarg);
				exit(0);
			}
			flags[3] = option; 
			n++;
			break;	
		case 'v':
			option = checkOptarg(optarg);
			if (option == -1){
				printf("%c is not a valid option for v\n", *optarg);
				exit(0);
			}
			flags[4] = option; 
			n++;
			break;	
		case 'c':
			option = checkOptarg(optarg);
			if (option == -1){
				printf("%c is not a valid option for c\n", *optarg);
				exit(0);
			}
			flags[5] = option; 
			n++;
			break;	
		case ':':
			printf("missing option argument for p\n");
			exit(0);
		case '?':
			n++;
			printf("%s is not an option\n", argv[n]);
			exit(0);
		}
	}
	
	ind = optind; 
	if (ind < argc){
		printf("Non-option argument: ");
		for (; ind < argc; ind++){
			printf("%s ", argv[ind]);
		}
		printf("\n");
		exit(0);
	}
}

void checkPid(char *optarg){
	int n = 0; 
	char c = optarg[0]; 

	while (c != '\0'){
		if (c < 48 || c > 57){
			printf("pid is not an integer\n"); 
			exit(0);
		}
		c = optarg[n];
		n++;
	}
}
int checkOptarg(char *optarg){
	if (optarg != NULL){
		if (*optarg == '-') {
			return 0;
		}
		else {
			return -1;
		}
	}
	else{
		return 1;
	}
}
