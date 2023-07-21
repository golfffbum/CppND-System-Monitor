#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float memTotal, memFree, memAvail, buffers; 
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
//       std::replace(line.begin(), line.end(), 'kB', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal"){
          memTotal = stof(value);
        }
        else if (key == "MemFree"){
          memFree = stof(value);
        }
        else if (key == "MemAvailable"){
          memAvail = stof(value);
        }
        else if (key == "Buffers"){
          buffers = stof(value);
        }
      }
    }
  }

  // % TotalUsedMem = (total - free) / total 
  return (memTotal - memFree)/memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idleTime;
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  //Total = Idle + NonIdle
  //   Idle = idle + iowait
  //   NonIdle = user + nice + system + irq + softirq + steal
  long total;
  vector<long> jiffies = GetJiffies();
  
  // sum all the jiffes together
  for (int state = kUser_; state <= kGuestNice_; ++state) {
    total += jiffies[state];
  }
  return total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long total;
  vector<long> jiffies = GetJiffies();
  
  // sum all the jiffes together
  for (int state = kUser_; state <= kGuestNice_; ++state) {
    total += jiffies[state];
  }
  //subtract the idle jiffies to get the active 
  return total - jiffies[kIdle_] - jiffies[kIOwait_];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<long> jiffies = GetJiffies();

  // sum the idle jiffies
  return jiffies[kIdle_] + jiffies[kIOwait_]; 
}

// // TODO: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { return {}; }

vector<long> LinuxParser::GetJiffies() {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    while (linestream >> key) {
      if (key == "cpu"){ //get line starting with "cpu"
		// Extract jiffies into vector of ints
        vector<long> numbers{std::istream_iterator<long>(linestream),
                             std::istream_iterator<long>()};

//       	while (linestream >> value) {
//           numbers.push_back(stoi(value));
// //           numbers.push_back(num);
//         }
        return numbers;
      }
    }
  }
    
  return vector<long>{};
}

// Get stat data associated with desired key
int LinuxParser::GetMemInfoFromKey(string desiredKey) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
      if (key == desiredKey){
        return stoi(value);
      }
    }
  }
    
  return stoi(value);
}
// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return GetMemInfoFromKey("processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return GetMemInfoFromKey("procs_running");
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line{""};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  
  while (std::getline(filestream, line)) {
    return line;
  }
  
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string value = GetPidStatusData(pid, "VmSize");  
  int mB = stoi(value) / 1000;
  return std::to_string(mB);
}

// Get status data associated with desired key
string LinuxParser::GetPidStatusData(int pid, string desiredKey)
{
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  while (std::getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    
    while (linestream >> key >> value) {
      if (key == desiredKey){
		return value;
      }
    }
  }
  return value; 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  return GetPidStatusData(pid, "Uid");  
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  
  string line, user, x, id;
  std::ifstream filestream(kPasswordPath);

  while (std::getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while(linestream >> user >> x >> id){
      if(id == uid){
        return user;
      }
    }
  }
  return string(); 
}

// Get stat filestream 
vector<string> LinuxParser::GetPidStatData(int pid)
{
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  vector<string> stat;
  
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    while (linestream >>value) {
      stat.push_back(value);
    }
  }
  return stat;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  vector<string> stat = GetPidStatData(pid);
  long processTime = stoul(stat[21]);
  long sysUptime = UpTime();
  
  // convert from clock ticks to sec 
  long time = sysUptime - (processTime / sysconf(_SC_CLK_TCK));
  
  return time;
}
