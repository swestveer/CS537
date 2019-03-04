///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #3
//
// Authors:          Shawn Westveer westveer@wisc.edu
//					Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "node.h"
#include "text_parsing.h"
#include<sys/stat.h>
#include <time.h>

char **array, **temp;
int c, b, pos = 0, arrayRow = 0, arrayCol = 0, tempRow = 0, 
tempCol = 0, target = 0, line = 0, countCol = 0, countRow = 0;
char *cmdToken, *targetToken, *replace, *replaceWith;
//delimiter values to be used with strtok, covers all types of whitespace
const char s[12] = " \t\r\v\f\n";

//Methond for getting Targets 
void getTarget(char* stng, int pos, char **array) {

    //check that the first index of the string is not a whitespace
    if( isspace(stng[0]) == 0){
        char *ch = (char*)malloc(sizeof(char));
        if (ch == NULL) {
            fprintf(stderr,"%s\n", "malloc failed");
            exit(1);
        }
        char *arraystring = (char*)malloc((BUFFERSIZE)*sizeof(char));
        if (arraystring == NULL) {
            fprintf(stderr,"%s\n", "malloc failed");
            exit(1);
        }
        //Flags first value of array. 
        //0 = target line, 1 = command line. 2 = lines to be placed into dictionary later
        //5 = default value, garbage line that will not be added to array
        strcpy(ch,"5");
        array[BUFFERSIZE*arrayRow+arrayCol] = ch;
	    arrayCol++;    
        for (int i=0; i<=pos; i++){
            //stops comments from being passed
            if(stng[i] == '#'){
                break;
            }
            //looks for lines to be entered into dictionary
            if(stng[i - 1] == ' ' && stng[i + 1] == ' ' && (stng[i] == '=' || (
            stng[i] == ':' && stng[i+1] == '=')))
            {
                target = 5000;
            }			
            arraystring[i] = stng[i];
		}
        //break up input string based on delimit values defined in s
        targetToken = strtok(arraystring, s);
        //iterate through arraystring through strtok command
        while( targetToken != NULL ) {
            //define length of word
            int length = strlen(targetToken);
            targetToken[length] = '\0';
            
            //allocate space of size length in current array index  
            array[BUFFERSIZE*arrayRow+arrayCol] = (char*)calloc((length),sizeof(char));
            if (array[BUFFERSIZE*arrayRow+arrayCol] == NULL) {
                fprintf(stderr,"%s\n", "calloc failed");
                exit(1);
            }
            //place delimted word into curren array index
            array[BUFFERSIZE*arrayRow+arrayCol] = targetToken;
            arrayCol++;
            //advance to next delimted word
            targetToken = strtok(NULL, s);
        }
    //set flag to target
	if(target == 1){
        strcpy(ch,"0");
		array[BUFFERSIZE*arrayRow] = ch;
	}
    //set flag to dictionary
	if(target >= 5000){
        strcpy(ch,"2");
		array[BUFFERSIZE*arrayRow] = ch;
	}
    //only advance arrayRow if string is valid
	if(strcmp( array[BUFFERSIZE*arrayRow], "5") != 0){
        	arrayRow++;            
	}else{
        strcpy(ch, "\0");
        for(int i=0; i<arrayCol; i++){
        array[BUFFERSIZE*arrayRow+i] = ch;
        }
    }
        //reset arrayCol to zero
        arrayCol = 0;
    }     
}

//Method for getting commands
void getCommands(char* strg, int pos, char **array){
    //check that string starts with a tab and is preceded with text 
    if(strg[0] == '\t' && isspace(strg[1]) == 0 && (strg[1]) != '#'){

        char *ch2 = (char*)malloc(sizeof(char));
        if (ch2 == NULL) {
            fprintf(stderr,"%s\n", "malloc failed");
            exit(1);
        }
        char *cmdstring = (char*)malloc(BUFFERSIZE*sizeof(char));
        if (cmdstring == NULL) {
            fprintf(stderr,"%s\n", "malloc failed");
            exit(1);
        }

        array[BUFFERSIZE*arrayRow+arrayCol] = (char*)malloc(sizeof(char));
        if (array[BUFFERSIZE*arrayRow+arrayCol] == NULL) {
            fprintf(stderr,"%s\n", "malloc failed");
            exit(1);
        }
        //set flag for commands
        strcpy(ch2,"1");
	    array[BUFFERSIZE*arrayRow+arrayCol] = ch2;
	    arrayCol++;

        for (int i=0; i<=pos; i++){
            //stops comments from being passed
            if(strg[i + 1] == '#'){
                break;
            }
            cmdstring[i] = strg[i+1];
        }
        //break up input string based on delimit values defined in s
        cmdToken = strtok(cmdstring, s);
        //iterate through arraystring through strtok command
        while( cmdToken != NULL ) {
            int length = strlen(cmdToken);
            cmdToken[length] = '\0';

            array[BUFFERSIZE*arrayRow+arrayCol] = (char*)calloc(length,sizeof(char));
            if (array[BUFFERSIZE*arrayRow+arrayCol] == NULL) {
                fprintf(stderr,"%s\n", "calloc failed");
                exit(1);
            }

            //place delimted word into curren array index
	        array[BUFFERSIZE*arrayRow+arrayCol] = cmdToken;
            arrayCol++;
            //advance to next delimted word
            cmdToken = strtok(NULL, s);
        }
        //advane to next arrayRow index
        arrayRow++;
        //reset arrayCol to zero
        arrayCol = 0;
    }
}
//Method for opening files
FILE* openFile(char *fileName){
    FILE *file_input;
    //if no -f 
    if(strcmp(fileName,"") == 0){
        file_input = fopen("makefile", "r");
        if(file_input == NULL) {
            file_input = fopen("Makefile", "r");
            if(file_input == NULL) {
                fprintf( stderr, "%s\n", "No makefile found");
                exit(0);
            }
        }
        return(file_input);
    //open -f specific makefile    
    }else{
        file_input = fopen(fileName, "r");
            if(file_input == NULL) {
                fprintf( stderr, "%s is not found\n", fileName);
                exit(0);
            }
        return(file_input);
    }
}
//Replaces variables in make file
void dictionary(char** arr){
    //replace variable in array with matching dictionary commands
    for (int i=0; i<arrayRow; i++){
        int j = 0;
        if(strcmp( arr[i*BUFFERSIZE+j], "1") == 0 || strcmp( arr[i*BUFFERSIZE+j], "0") == 0){
            while ( arr[i*BUFFERSIZE+j] != NULL ) {
                replace = arr[i*BUFFERSIZE+j];
                int length = strlen(replace);

                if(replace[0] == '$'){
                    int k = 0;
                    for(int i=0; i<length; i++){
                        if(replace[i] != '$' && replace[i] != '(' && replace[i] != ')'){
                            replace[k] = replace[i];
                            k++;
                        }
                    }
                    replace[k] = '\0';
                    length = strlen(replace);
                    //find dictionary enteries in array using flag 2
                    for (int x=0; x<arrayRow; x++){
                        int y = 0;
                        //if flag = 2 iterate through to check for matches
                        if(strcmp( arr[x*BUFFERSIZE+y], "2") == 0){
                            while ( arr[x*BUFFERSIZE+y] != NULL ) {
                                replaceWith = arr[x*BUFFERSIZE+y];
                                int rLength = strlen(replaceWith);

                                //match command entry with dictionary entry
                                if(strcmp( replace, replaceWith) == 0){

                                    b = y;
                                    b++;
                                    //move to first dictionary entry after '='
                                    while (arr[x*BUFFERSIZE+b] != NULL ) {
                                        if(strcmp(arr[x*BUFFERSIZE+b], "=") == 0){
                                            b++;
                                            //add dictionary to temp
                                            while ( arr[x*BUFFERSIZE+b] != NULL ) {
                                        
                                                temp[BUFFERSIZE*tempRow+tempCol] = (char*)calloc(rLength,sizeof(char));
                                                if (temp[BUFFERSIZE*tempRow+tempCol] == NULL) {
                                                    fprintf(stderr,"%s\n", "calloc failed");
                                                    exit(1);
                                                }
                                                temp[tempRow*BUFFERSIZE+tempCol] = arr[x*BUFFERSIZE+b];
                                                tempCol++;
                                                b++;
                                            }
                                            break;
                                        }
                                        b++;
                                    }
                                }
                                y++;
                            }
                        }
                    }                    
                }else{
                    temp[tempRow*BUFFERSIZE+tempCol] = arr[i*BUFFERSIZE+j];
                    tempCol++;
                    b++;
                }
                j++;
            }
            tempCol = 0;
            if(temp[tempRow*BUFFERSIZE+tempCol] != NULL){
                tempRow++;
            }                 
        }
    }
}
//Parse a selected file
char **text_parsing(int *nRow, char *fname){
    char *string = (char*)malloc(BUFFERSIZE*sizeof(char));
    FILE *file_input;
    file_input = openFile(fname);
    //Read input from make file character by character
    do{

        c = fgetc(file_input);
        countCol++;
        if(countCol >= BUFFERSIZE){
            fprintf( stderr, "Line#%d exceeds BUFFERSIZE->",countRow);
            exit(0);

        }
        if(c == '\n' ){
            countRow++;
            countCol = 0;
        }
    }while(c != EOF);
    fclose(file_input);

    array = (char**)calloc(countRow*BUFFERSIZE,sizeof(char*));
    temp = (char**)calloc(countRow*BUFFERSIZE,sizeof(char*));

    file_input = openFile(fname);
    //iterate through open file, character by character
    do {
        //all string start as invalid
        int valid = 1;
        c = fgetc(file_input); 
        if (c == '\n'){
            line++;
        }
        //Check for when an end of line has been reached
        if (c == '\n' && pos > 0 && c != EOF){
            
            string[pos] = c;

            //set comment lines as vaild to prevent them being read as invalid input
            if(string[0] == '#'){
                valid = 0;
            }
            //string is it meets the requirement of a target
            if(isspace(string[0]) == 0 && pos > 0 && target == 1){
                valid = 0;
                getTarget(string, pos, array);
            }
            //string is it meets the requirement of a command
            if((string[0] == '\t' && isspace(string[1]) == 0) && string[1] != '#'){
                valid = 0;
                getCommands(string, pos, array);
            }
            //if a line is not a comment, target or command it is invalid
            if(valid == 1 && pos > 1){
                fprintf( stderr, "Line#%d is invalid ->%s",line, string);
                exit(0);
            }
            //clear string
            for (int i=0; i<=pos; i++){
                string[i] = '\0';
            }
            target = 0;
            pos = 0;
		}
        //if c is not end of line and EOF add to string
        if (c != '\n' && c != EOF ){
            //check for variable lines
            if(c == '='){ 
                target++;
            }
            //check for target lines
            if(c == ':'){
                strcat(string, " ");
                pos++;
                target++;
            //if c is not end of line add to string
            }else if(c != '\n'){
                string[pos] = c;
		        pos++;
            }
        }
        //when EOF is reached add proccess final line and exit
        if (c == EOF){
            //set comment lines as vaild to prevent them being read as invalid input
            if(string[0] == '#'){
                valid = 0;
            }
            //string is it meets the requirement of a target
            if(isspace(string[0]) == 0 && pos > 0 && target == 1){
                    valid = 0;
		    getTarget(string, pos, array);
            }
            //string is it meets the requirement of a command
            if((string[0] == '\t' && isspace(string[1]) == 0)){
                    valid = 0;
		    getCommands(string, pos, array);
            }

            if(valid == 1 && pos > 1){
                fprintf( stderr, "Line#%d is invalid ->%s",line, string);
                exit(0);
            }
            //if a line is not a comment, target or command it is invalid
            break;
        }
    }while(1);
    
    fclose(file_input);  
    dictionary(array);
    *nRow = countRow; 
    return temp;
}
