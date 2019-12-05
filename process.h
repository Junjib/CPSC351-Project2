#ifndef PROCESS_H
#define PROCESS_H

struct Process
{
  int aTime;
  int size;
  int cTime;
  int pid;
  int tMem;
  int active = 0;
  int totalCompletionTime = 0;
};

#endif
