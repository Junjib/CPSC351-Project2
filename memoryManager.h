#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include "process.h"

using namespace std;

void readFile(vector<Process>& processes, int &numOfProcesses);

void readFile(vector<Process>& processes, int &numOfProcesses)
{
  ifstream read;
  int pid = 0, aTime = 0, cTime = 0, size = 0, discard = 0, num = 0;
  Process p;

  read.open("in1.txt");
  read >> numOfProcesses;
  while(!read.eof())
  {
    read >> p.pid;
    read >> p.aTime;
    read >> p.cTime;
    read >> discard;
    if(discard == 1)
    {
      read >> p.size;
    }
    else
    {
      for(int i = 0; i < discard; i++)
      {
        read >> size;
        num += size;
        p.size = num;
      }
      num = 0;
    }
    processes.push_back(p);
  }
  processes.pop_back();
  read.close();
}
