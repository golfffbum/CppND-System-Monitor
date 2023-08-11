#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
    Process(int pid) : pid_(pid){
      // set variables so only have to access file once
      command_ = LinuxParser::Command(pid_);
      ram_ = LinuxParser::Ram(pid_);
      uid_ = LinuxParser::Uid(pid_);
      user_ = LinuxParser::User(pid_);
      uptime_ = LinuxParser::UpTime(pid_);
    }

  int Pid();                             
  std::string User();                    
  std::string Command();                 
  double SetCpuUtilization();
  double CpuUtilization();
  std::string Ram();                     
  long int UpTime();                     
  bool operator>(Process const& a) const;

  // private members
 private:
  int pid_{0};
  std::string command_{};
  std::string ram_{};
  std::string user_{};
  long int uptime_{0};
  std::string uid_{};
  double cpu_usage_{0};
};

#endif