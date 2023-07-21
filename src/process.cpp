#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  long utime, stime, cutime, cstime, starttime;
  float hertz = sysconf(_SC_CLK_TCK);
  //proc/uptime 
  long uptime = LinuxParser::UpTime();
  //proc/pid/stat
  vector<string> stat = LinuxParser::GetPidStatData(pid_);
  utime = stol(stat[13]);
  stime = stol(stat[14]);
  cutime = stol(stat[15]);  
  cstime = stol(stat[16]);
  starttime = stol(stat[21]);
  
  // get total = utime + stime + cutime + cstime
  long totaltime = utime + stime + cutime + cstime;
  // get elapsed time
  float seconds = uptime - (starttime / hertz);
  // convert to percentage
  float cpu_usage = 100 * ((totaltime / hertz) / seconds);
  
  //update member var 
  cpu_usage_ = cpu_usage;
  
  return cpu_usage; 
}

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return ram_; }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator>(Process const& a) const { 
   return cpu_usage_ > a.cpu_usage_;
//   return true; 
}
