
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cmath>
#include "process.h"
#include "frames.h"

using namespace std;

void readFile(vector<Process>& processes, int &numOfProcesses);
void askUser(int &memSize, int &pageSize, int MAX_MEMORY);
void mainLoop(int &t, vector<Process>& inputQueue, vector<Process>& processes, int MAX_TIME, int MAX_MEMORY, int memSize, int pageSize, vector<Frames>& memMap);
void addToQueue(int &t, vector<Process>& inputQueue, vector<Process>& processes, vector<Frames>& memMap, int memSize, int pageSize, int &leftOverMem);
void moveToMemory(vector<Frames>& memMap, vector<Process>& inputQueue, int pageSize, int memSize, int &leftOverMem);
void printMap(vector<Frames>& memMap, int memSize, int pageSize, int &leftOverMem);
void sumPage(int pageSize, int page, int& totalMemUsed, int& calls, int numOfPages);

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

void askUser(int &memSize, int &pageSize, int MAX_MEMORY)
{
  cout << "Enter the memory size: ";
  cin >> memSize;
  while(memSize > MAX_MEMORY)
  {
    cout << "ERROR: memory size can not be greater than " << MAX_MEMORY << endl;
    cout << "Enter the memory size: ";
    cin >> memSize;
  }
  cout << "Enter the page size(1: 100 or 2: 200 or 3: 400): ";
  cin >> pageSize;
  while(pageSize < 1 || pageSize > 3 )
  {
    cout << "ERROR: page size must be 1 or 2 or 3\n";
    cout << "Enter the page size(1: 100 or 2: 200 or 3: 400): ";
    cin >> pageSize;
  }
}

void mainLoop(int &t, vector<Process>& inputQueue, vector<Process>& processes, int MAX_TIME, int MAX_MEMORY, int memSize, int pageSize, vector<Frames>& memMap)
{
  int queueCounter = 0;
  int leftOverMem = memSize;
  while(true)
  {
    addToQueue(t, inputQueue, processes, memMap, memSize, pageSize, leftOverMem);
    moveToMemory(memMap, inputQueue, pageSize, memSize, leftOverMem);

    t++;
    if(t > MAX_TIME)
    {
      cout << "ERROR: max time exceeded!\n";
      break;
    }
    if(inputQueue.size() == processes.size())
    {
      break;
    }
  }
}

void addToQueue(int &t, vector<Process>& inputQueue, vector<Process>& processes, vector<Frames>& memMap, int memSize, int pageSize, int &leftOverMem)
{
  int count = 0;
  for(int i = 0; i < processes.size(); i++)
  {
    if(t == processes[i].aTime)
    {
      if(count == 0)
      {
        cout << "t = " << t << ": ";
        cout << "Process " << processes[i].pid << " arrives\n";
      }
      inputQueue.push_back(processes[i]);
      if(count > 0)
      {
        cout << "\tProcess " << processes[i].pid << " arrives\n";
      }
      cout << "\t Input Queue:[";
      for(int j = 0; j < inputQueue.size(); j++)
      {
        cout << inputQueue[j].pid << " ";
      }
      cout << "]\n";
      printMap(memMap, memSize, pageSize, leftOverMem);
      count++;
    }
  }
}

void moveToMemory(vector<Frames>& memMap, vector<Process>& inputQueue, int pageSize, int memSize, int &leftOverMem)
{
  Frames frameObj;
  float roundPage = 0.0;

  if(pageSize == 1)
  {
    pageSize = 100;
  }
  else if(pageSize == 2)
  {
    pageSize = 200;
  }
  else if(pageSize == 3)
  {
    pageSize = 400;
  }
  roundPage = pageSize;

  if(inputQueue.size() != 0)
  {
    for(int i = 0; i < inputQueue.size(); i++)
    {
      if(inputQueue[i].size <= memSize && inputQueue[i].size <= leftOverMem)
      {
        frameObj.proc = inputQueue[i].pid;
        frameObj.mem = inputQueue[i].size;
        frameObj.pages = round(frameObj.mem / roundPage);
        leftOverMem -= frameObj.mem;
        memMap.push_back(frameObj);
        cout << "\t MM moves process " << frameObj.proc << " to memory\n";
        printMap(memMap, memSize, pageSize, leftOverMem);
        inputQueue.erase(inputQueue.begin());
      }
      // Maybe here we can add a conditional to remove processes that are too large
      // for the memory manager from the input queue.
    }
  }
}

void printMap(vector<Frames>& memMap, int memSize, int pageSize, int &leftOverMem)
{
  int totalMemUsed = 0;
  int calls = 0;
  cout << "\t Memory map:\n";
  if(memMap.size() == 0)
  {
    cout << "\t\t0 - " << (memSize - 1) << ": Free frame(s)\n";
  }
  else
  {
    for(int i = 0; i < memMap.size(); i++)
    {
      calls = 0;
      for(int j = 1; j <= memMap[i].pages; j++)
      {
        sumPage(pageSize, j, totalMemUsed, calls, memMap[i].pages);
        cout << " Process " << memMap[i].proc << ", Page " << j << endl;
      }
    }
    if(leftOverMem != 0)
    {
      cout << "\t\t" << totalMemUsed << " - " << (memSize - 1) << ": Free frames(s)\n";
    }
  }
}

void sumPage(int pageSize, int page, int& totalMemUsed, int& calls, int numOfPages)
{
  int lowEnd = 0;
  int highEnd = 0;

  if(pageSize == 1)
  {
    pageSize = 100;
  }
  else if(pageSize == 2)
  {
    pageSize = 200;
  }
  else if(pageSize == 3)
  {
    pageSize = 400;
  }

  if(totalMemUsed != 0)
  {
    highEnd = ((pageSize * page) - 1) + totalMemUsed;
    lowEnd = (highEnd - pageSize) + 1;
  }
  else
  {
    highEnd = (pageSize * page) - 1;
    lowEnd = (highEnd - pageSize) + 1;
  }
  cout << "\t\t" << lowEnd << " - " << highEnd << ":";
  calls++;
  if(calls == numOfPages)
  {
    totalMemUsed = highEnd + 1;
  }
}
