#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 

  // clear processes list to rebuild
  processes_.clear();

  vector<int> pids = LinuxParser::Pids();
  for (int p : pids)
  {
    processes_.push_back(Process(p));
  }
  
  // set cpu usage 
  for (auto process : processes_)
  {
    process.SetCpuUtilization();
  }
  
  // sort processes
  //use lambda, [capture list](parameters){ function body}
    // capture list []: what oustide variable are accessible in the lambda function
    // params (): decalre paramters the lambda function will take in 
    // function body {}: the actual code the function will execute
  sort(
      processes_.begin(),
      processes_.end(),
      []
      (const Process a, const Process b)
      {return a > b;}
    );

  return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() {
  return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}
