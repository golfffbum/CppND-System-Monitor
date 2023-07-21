#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // add up number of jiffies, see urls
    // add up amount of time or # of jiffies / how much time has passed
  // read in numbers and do division to get percentage 
  // these numbers rep info from the beginning of time, can capture a more recent measurment, right now vs entire lifetime 


  long total, idle, totalD, idleD;
  float cpuPercent = 0.0;
  // get current
  total = LinuxParser::Jiffies();
  idle = LinuxParser::IdleJiffies();

  // differentiate: actual value minus the previous one
  totalD = total - prevTotal;
  idleD = idle - prevIdle;

  //   CPU_Percentage = (totald - idled)/totald
  if (totalD != 0)
  {
    cpuPercent = static_cast<float>(totalD - idleD) / totalD; 
  }
  
  // save previous 
  prevTotal = total;
  prevIdle = idle;

  return cpuPercent; 
}
