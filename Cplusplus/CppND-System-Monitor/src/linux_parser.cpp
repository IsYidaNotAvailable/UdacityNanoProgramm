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

// DONE: Read and return the system memory utilization
// Implemented in system.cpp
// float LinuxParser::MemoryUtilization() { return 0.0; }

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line_uptime;
  string uptime_total;
  string uptime_idle;
  string value;
  vector<string> values;
  
  std::ifstream stream1(kProcDirectory + kUptimeFilename);
  if (stream1.is_open()) {
    std::getline(stream1, line_uptime);
    std::istringstream linestream(line_uptime);
    linestream >> uptime_total >> uptime_idle;
  }

  if (pid != -1) {
    std::ifstream stream2(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream2.is_open()) {
      std::getline(stream2, line_uptime);
      std::istringstream linestream(line_uptime);
      while (linestream >> value) {
        values.emplace_back(value);
      }
      return std::stol(values[21])/sysconf(_SC_CLK_TCK) - std::stol(uptime_total);
    }
  }
  
  return std::stol(uptime_total);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { return UpTime(-1); }

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long uptime = UpTime(-1);
  return uptime * sysconf(_SC_CLK_TCK);
}

long LinuxParser::SystemJiffies(int pid = -1, bool aktiv = true) {
  string cpu_string;
  string j_user;
  string j_nice;
  string j_system;
  string j_idle;
  string j_iowait;
  string j_irq;
  string j_softirq;
  string j_steal;

  std::ifstream stream(pid == -1? kProcDirectory + kStatFilename : kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, cpu_string);
    std::istringstream linestream(cpu_string);
    linestream >> cpu_string >> j_user >> j_nice >> j_system >> j_idle >> j_iowait >> j_irq >> j_softirq >> j_steal;
  }
  // Active jiffies = kUser_ + kNice_ + kSystem_ + kIRQ_ + kSoftIRQ_ + kSteal_
  long n_active = std::stol(j_user) + std::stol(j_nice) + std::stol(j_system) + std::stol(j_irq) + std::stol(j_softirq) + std::stol(j_steal);
   // Idle jiffies = kIdle_ + kIOwait_
  long n_idle = std::stol(j_idle) + std::stol(j_iowait);

  if (aktiv) { return n_active; }
  else { return n_idle; }
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { return SystemJiffies(pid, true); }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return SystemJiffies(-1, true); }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return SystemJiffies(-1, false); }

// DONE: Read and return CPU utilization
// Implemented in process.cpp
// vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE: Read and return the total number of processes
// Implemented in system.cpp
// int LinuxParser::TotalProcesses() { return 0; }

// DONE: Read and return the number of running processes
// Implemented in system.cpp
// int LinuxParser::RunningProcesses() { return 0; }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmdstring;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  std::getline(stream, cmdstring);
  return cmdstring;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string s_ram;
  string n_ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (s_ram != "VmSize:") {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> s_ram >> n_ram;
  }
  return std::to_string(stol(n_ram) / 1000);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string s_uid;
  string n_uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (s_uid != "Uid:") {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> s_uid >> n_uid;
  }
  return n_uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string s_user;
  string bla;
  string key;
  
  string n_uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> s_user >> bla >> key) {
        if (key == n_uid) { return s_user; }
      }
    }
  }
  return s_user;
}


