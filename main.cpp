#include "memoryManager.h"
#include "process.h"
#include "frames.h"

using namespace std;

const int MAX_TIME = 100000;
const int MAX_MEMORY = 30000;

int main()
{
  int t = 0, pageSize, memSize, numOfProcesses = 0;
  vector<Process> processes;
  vector<Process> inputQueue;
  vector<Frames> memMap;

  readFile(processes, numOfProcesses);
  askUser(memSize, pageSize, MAX_MEMORY);
  cleanUp(processes, memSize);

  int pageSizePlus = 0;
  if(pageSize == 1)
  {
    pageSizePlus = 100;
  }
  else if(pageSize == 2)
  {
    pageSizePlus = 200;
  }
  else if(pageSize == 3)
  {
    pageSizePlus = 400;
  }
  float memoryMapSize = floor(memSize / pageSizePlus);

  sumPage(memMap, pageSize, memoryMapSize);
  cout << "\n\n";
  mainLoop(t, inputQueue, processes, MAX_TIME, MAX_MEMORY, memSize, pageSize, memMap);

  return 0;
}
