///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #1
//
// Authors:          Shawn Westveer westveer@wisc.edu
//									 Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "getinfo.h"

void getinfo(struct Process * procs, int nProc, uid_t uid, int pid, int * pCount) {

  int nCharShort = 256;
  int nCharLong = 2000;

  char *ptr, *str;
  char * procID = "Uid:";
  long dir_ret, uid_cmpr;
  DIR * primary, * sub;
  struct dirent * process_dir, * process_sub_dir;
  int count = 0, cmpr, pid_cmpr, wcount;

  uid_cmpr = -1;

  char *pbuffer = (char*)malloc(nCharShort*sizeof(char)); 
  char *status = (char*)malloc(nCharShort*sizeof(char)); 
  char *status_word = (char*)malloc(nCharShort*sizeof(char)); 
  char *stat = (char*)malloc(nCharShort*sizeof(char)); 
  char *stat_word = (char*)malloc(nCharShort*sizeof(char)); 
  char *statm = (char*)malloc(nCharShort*sizeof(char)); 
  char *statm_word = (char*)malloc(nCharShort*sizeof(char)); 
  char *cmdline = (char*)malloc(nCharLong*sizeof(char)); 
  char *longBuffer = (char*)malloc(nCharLong*sizeof(char)); 


  if ((primary = opendir("/proc")) == NULL) {
    perror("opendir() error");
  } else {
    while ((process_dir = readdir(primary)) != NULL) {
      if ((dir_ret = strtol(process_dir -> d_name, & ptr, 10)) != 0) {
        pid_cmpr = atoi(process_dir -> d_name);
        if ((pid_cmpr == pid) || (pid == -1)) {
          //appends process ID onto /proc/
          sprintf(pbuffer, "/proc/%s", process_dir -> d_name);
          //appends process ID onto /proc/<processID>/stat
          sprintf(stat, "/proc/%s/%s", process_dir -> d_name, "stat");
          //appends process ID onto /proc/<processID>/statm
          sprintf(statm, "/proc/%s/%s", process_dir -> d_name, "statm");
          //appends process ID onto /proc/<processID>/cmdline
          sprintf(cmdline, "/proc/%s/%s", process_dir -> d_name, "cmdline");
          sprintf(status, "/proc/%s/%s", process_dir -> d_name, "status");

	  sub = opendir(pbuffer);
          if (sub  == NULL) {
            perror("opendir() error");
          } else {
            //Iterates through processes
            if ((process_sub_dir = readdir(sub)) != NULL) {

              FILE * status_file = fopen(status, "r");
              //Finds the UID of the process	
              if (status_file != NULL) {
                while (fscanf(status_file, "%50s", status_word) != EOF) {
                  cmpr = strcmp(status_word, procID);
                  if (cmpr == 0) {
                    fscanf(status_file, "%55s", status_word);
                    uid_cmpr = strtol(status_word, NULL, 10);
                    break;
                  }
                }
              }

              fclose(status_file);
              //Check that process either matched requested pid
              //or pid flad is -1 and process uid matches user
              if ((pid_cmpr == pid) || ((pid == -1) && (uid_cmpr == uid))) {
                // checks if number of process exceeds allocation
                if (count == nProc) {
                  perror("Number of process exceeds memory allocation.\n");
                  exit(0);
                }
                //adds process ID to procs[count]
                strcpy(procs[count].pid, process_dir -> d_name);

                //iterates through stat file and pulls 3rd, 14th, and 15th word
                wcount = 0;
                FILE * stat_file = fopen(stat, "r");
                if (stat_file != NULL) {
                  while (fscanf(stat_file, "%50s", stat_word) != EOF) {
                    if (wcount == 2) {
                      strcpy(procs[count].state, stat_word);
                    }
                    if (wcount == 13) {
                      strcpy(procs[count].utime, stat_word);
                    }
                    if (wcount == 14) {
                      strcpy(procs[count].stime, stat_word);
                    }
                    wcount++;
                  }
                }
                fclose(stat_file);

                FILE * statm_file = fopen(statm, "r");
                if (statm_file != NULL) {
                  while (fscanf(statm_file, "%50s", statm_word) != EOF) {
                    wcount = 0;
                    if (wcount == 0) {
                      strcpy(procs[count].vmem, statm_word);
                      break;
                    }
                    wcount++;
                  }
                }
                fclose(statm_file);

                str = longBuffer;
                char c;
                FILE * cmdline_file = fopen(cmdline, "r");
		for (int i=0; i<nCharLong; i++){
			longBuffer[i]='\0';
		}
		int j = 0; 
                if (cmdline_file != NULL) {
                  c = fgetc(cmdline_file);
                  while (c != EOF) {
                    	strcat(str, & c);
                    	str[j] = c;
			j++;
			c = fgetc(cmdline_file);
                  }
		  str[j] = '\0';
                  strcpy(procs[count].cmdline, str);
                }
                fclose(cmdline_file);
                count++;
              }
            }
		closedir(sub);
          }
        }
      }
    }
    closedir(primary);
  }
  *pCount = count;

  free(pbuffer);
  free(status); 
  free(status_word); 
  free(stat); 
  free(stat_word); 
  free(statm); 
  free(statm_word); 
  free(cmdline); 
  free(longBuffer); 
}
