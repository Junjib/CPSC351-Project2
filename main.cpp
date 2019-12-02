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

  /*for(int i = 0; i < processes.size(); i++)
  {
    cout << processes[i].pid << " " << processes[i].aTime << endl;
  }*/

  askUser(memSize, pageSize, MAX_MEMORY);
  /*cout << "memSize: " << memSize << endl;
  cout << "pageSize: " << pageSize << endl;*/

  mainLoop(t, inputQueue, processes, MAX_TIME, MAX_MEMORY, memSize, pageSize, memMap);

  return 0;
}
