///////////////////////////////////////////////////////////////////////////////
// Title:            Programming Assignment #1
//
// Authors:          Shawn Westveer westveer@wisc.edu
//									 Jing-Yao Chen jchen478@wisc.edu							
//////////////////////////// 80 columns wide //////////////////////////////////
#ifndef __537ps__getinfo__
#define __537ps__getinfo__

#include <stdlib.h>

extern int nCharShort; 
extern int nCharLong; 

typedef struct Process {
  char  pid[256];
  char  state[256];
  char  utime[256];
  char  stime[256];
  char  vmem[256];
  char  cmdline[2000];
} Process;  

void getinfo(struct Process *procs, int nProc, uid_t uid, int pid, int *pCount);

#endif /* defined(__537ps__getinfo__) */
