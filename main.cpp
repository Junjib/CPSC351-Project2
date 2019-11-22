#include "memoryManager.h"
#include "process.h"

using namespace std;

int main()
{
  int t = 0, pageSize, memSize, numOfProcesses = 0;
  vector<Process> processes;

  readFile(processes, numOfProcesses);

  cout << processes.size() << endl;
  for(int i = 0; i < processes.size(); i++)
  {
    cout << processes[i].pid << endl;
    cout << processes[i].aTime << endl;
    cout << processes[i].cTime << endl;
    cout << processes[i].size << endl;
    cout << endl << endl;
  }
  return 0;
}
