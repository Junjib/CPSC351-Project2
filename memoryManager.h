
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>
#include "process.h"
#include "frames.h"

using namespace std;

void readFile(vector<Process>& processes, int &numOfProcesses);
void askUser(int &memSize, int &pageSize, int MAX_MEMORY);
void mainLoop(int &t, vector<Process>& inputQueue, vector<Process>& processes, int MAX_TIME, int MAX_MEMORY, int memSize, int pageSize, vector<Frames>& memMap);
void addToQueue(int &t, vector<Process>& inputQueue, vector<Process>& processes, vector<Frames>& memMap, int memSize, int pageSize, int &leftOverMem, int& queueCounter);
void moveToMemory(vector<Frames>& memMap, vector<Process>& inputQueue, vector<Process>& processes, int pageSize, int memSize, int &leftOverMem, int &t);
void printMap(vector<Frames>& memMap, int memSize, int pageSize);
void sumPage2(vector<Frames>& memMap, int pageSize, int memoryMapSize);
void processComplete(vector<Frames>& memMap, vector<Process>& processes, vector<Process>& inputQueue, int &t, int memSize, int pageSize, int &leftOverMem, int &queueCounter);
void computeTurnAround(vector<Process>& processes);
void cleanUp(vector<Process>& processes, int memSize);

void readFile(vector<Process>& processes, int &numOfProcesses)
{
  ifstream read;
  int pid = 0, aTime = 0, cTime = 0, size = 0, discard = 0, num = 0;
  Process p;
  string fileName = "";

  cout << "Enter a filename: ";
  cin >> fileName;

  read.open(fileName);
  while(!read.is_open())
  {
    read.close();
    cout << "File does not exist\n";
    cout << "Enter a filename: ";
    cin >> fileName;
    read.open(fileName);
  }
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
    addToQueue(t, inputQueue, processes, memMap, memSize, pageSize, leftOverMem, queueCounter);
    moveToMemory(memMap, inputQueue, processes, pageSize, memSize, leftOverMem, t);
    processComplete(memMap, processes, inputQueue, t, memSize, pageSize, leftOverMem, queueCounter);

    if(queueCounter == processes.size())
    {
      computeTurnAround(processes);
      break;
    }

    t++;
    if(t > MAX_TIME)
    {
      cout << "ERROR: max time exceeded!\n";
      break;
    }
  }
}

void addToQueue(int &t, vector<Process>& inputQueue, vector<Process>& processes, vector<Frames>& memMap, int memSize, int pageSize, int &leftOverMem, int& queueCounter)
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
      printMap(memMap, memSize, pageSize);
      count++;
    }
  }
}

void moveToMemory(vector<Frames>& memMap, vector<Process>& inputQueue, vector<Process>& processes, int pageSize, int memSize, int &leftOverMem, int &t)
{
  Frames frameObj;
  float roundPage = 0.0, pages;
  int counter = 0, pageNum = 1;
  int inputSize = inputQueue.size();
  int inputPos = 0;

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
    for(int i = 0; i < inputSize; ++i)
    {
      if(inputQueue[i].size <= leftOverMem)
      {
        pages = ceil(inputQueue[i].size / roundPage);
        counter = 0;
        pageNum = 0;
        leftOverMem -= (roundPage * pages);

        for(int j = 0; j < memMap.size(); j++)
        {
          if(memMap[j].validBit != 1 && counter != pages)
          {
            counter++;
            pageNum++;
            memMap[j].proc = inputQueue[i].pid;
            memMap[j].page = pageNum;
            memMap[j].validBit = 1;
          }
        }
        if(counter == pages)
        {
          cout << "\t MM moves process " << inputQueue[i].pid << " to memory\n";
          printMap(memMap, memSize, pageSize);
        }
        for(int k = 0; k < processes.size(); k++)
        {
          if(inputQueue[i].pid == processes[k].pid)
          {
            processes[k].tMem = t;
            processes[k].active = 1;
          }
        }
        inputQueue.erase(inputQueue.begin() + inputPos);
      }
      else
      {
        inputPos++;
      }
    }
  }
}

void printMap(vector<Frames>& memMap, int memSize, int pageSize)
{
  int totalFreeHigh = 0, totalFreeLow = 0, lowCount = 0;

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

  cout << "\t Memory map:\n";
  for(int i = 0; i < memMap.size(); i++)
  {
    if(memMap[i].validBit == 1)
    {
      cout << "\t\t" <<  memMap[i].lowEnd << " - " << memMap[i].highEnd << ":";
      cout << " Process " << memMap[i].proc << ", Page " << memMap[i].page << endl;
    }
    else if(memMap[i].validBit != 1)
    {
      if(lowCount == 0)
      {
        totalFreeLow = memMap[i].lowEnd;
        lowCount = 1;
      }
      while(memMap[i].validBit != 1 && i < memMap.size())
      {
        totalFreeHigh = memMap[i].highEnd;
        i++;
      }
      i--;
      lowCount = 0;
      cout << "\t\t" << totalFreeLow << " - " << totalFreeHigh << ": Free frames(s)\n";
    }
  }
}

void sumPage(vector<Frames>& memMap, int pageSize, int memoryMapSize)
{
  Frames frameObj;

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

  for(int i = 1; i <= memoryMapSize; i++)
  {
    frameObj.highEnd = ((pageSize * i) - 1);
    frameObj.lowEnd = (frameObj.highEnd - pageSize) + 1;
    memMap.push_back(frameObj);
  }
}

void processComplete(vector<Frames>& memMap, vector<Process>& processes, vector<Process>& inputQueue, int &t, int memSize, int pageSize, int &leftOverMem, int &queueCounter)
{
  int counter = 0;
  for(int i = 0; i < processes.size(); i++)
  {
    if(processes[i].active == 1 && (processes[i].cTime + processes[i].tMem) == t)
    {
      processes[i].active = 0;
      queueCounter++;
      processes[i].totalCompletionTime = (processes[i].cTime + processes[i].tMem);
      leftOverMem += processes[i].size;
      if(counter == 0)
      {
        cout << "t = " << t << ": Process " << processes[i].pid << " completes\n";
        counter++;
      }
      else
      {
        cout << "\t  Process " << processes[i].pid << " completes\n";
      }
      for(int j = 0; j < memMap.size(); j++)
      {
        if(memMap[j].proc == processes[i].pid)
        {
          memMap[j].validBit = -1;
        }
      }
      printMap(memMap, memSize, pageSize);
    }
  }
  moveToMemory(memMap, inputQueue, processes, pageSize, memSize, leftOverMem, t);
}

void computeTurnAround(vector<Process>& processes)
{
  float total = 0, procTurnAround = 0;

  for(int i = 0; i < processes.size(); i++)
  {
    procTurnAround = (processes[i].totalCompletionTime - processes[i].aTime);
    total += procTurnAround;
  }
  cout << "Average turnaround: " << fixed << setprecision(2) << (total / processes.size());
  cout << " (" << static_cast<int>(total) << " / " << processes.size() << ")\n";
}

void cleanUp(vector<Process>& processes, int memSize)
{
  int pos = 0;
  int vSize = processes.size();
  for(int i = 0; i < vSize; i++)
  {
    if(processes[i].size > memSize)
    {
      processes.erase(processes.begin() + pos);
    }
    else
    {
      pos++;
    }
  }
}
