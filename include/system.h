#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
 public:
  Processor& Cpu();                 
  std::vector<Process>& Processes();
  float MemoryUtilization();        
  long UpTime();                    
  int TotalProcesses();             
  int RunningProcesses();           
  std::string Kernel();             
  std::string OperatingSystem();    

 private:
  Processor cpu_ = Processor();
  std::vector<Process> processes_ = {};
};

#endif