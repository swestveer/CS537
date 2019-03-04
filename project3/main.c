///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #3
//
// Authors:          Shawn Westveer westveer@wisc.edu
//					Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "node.h"
#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "text_parsing.h"
#include "proc_creation_prog_exe.h"
 
int main(int argc, const char * argv[]) {
	
	char **input = NULL;

	char *File = (char*)calloc(BUFFERSIZE,sizeof(char));
	char *Target = (char*)calloc(BUFFERSIZE,sizeof(char));
	char flag[3] = "-f";
	
	strcpy(Target,"");
	strcpy(File,"");
	int ind = -1;
	// loop through arguement
	for (int i=1; i<argc; i++){
		//  if it's not a flag -f 
		//  then it's token name
		if (strcmp(argv[i],flag) != 0){
			strcpy(Target,argv[i]);
		}
		// if it's flag -f
		// get index of its argument
		else{
			ind = i+1;		
			i++;
		}
	}
	// if -f has no argument
	if (ind == argc){
		fprintf(stderr, "-f has no arguments\n");
		exit(1);
	}
	// if index is specified
	else if(ind != -1){
		strcpy(File,argv[ind]);
	}	
	
	// parse makefile into input
	int nRow = 0; 
	input = text_parsing(&nRow,File);

	// build graph
	struct Node *head = build_spec_graph(input,Target,nRow);

	// generate list of targets needing recompiling 
	int nTar = 0; // number of targets needed 
	struct Node **tar = build_spec_repr(head, input, &nTar,Target);

	// execute commands using execvp
	proc_creation_prog_exe(tar,nTar,input);

	return 0;
}
