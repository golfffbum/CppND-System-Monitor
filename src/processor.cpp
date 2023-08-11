#include "processor.h"

// Return the aggregate CPU utilization
double Processor::Utilization() { 
  long total, idle, totalD, idleD;
  double cpuPercent = 0.0;
  
  // get current
  total = LinuxParser::Jiffies();
  idle = LinuxParser::IdleJiffies();

  // differentiate: actual value minus the previous one
  totalD = total - prevTotal;
  idleD = idle - prevIdle;

  //   CPU_Percentage = (totald - idled)/totald
  if (totalD != 0)
  {
    cpuPercent = static_cast<double>(totalD - idleD) / totalD; 
  }
  
  // save previous 
  prevTotal = total;
  prevIdle = idle;

  return cpuPercent; 
}
