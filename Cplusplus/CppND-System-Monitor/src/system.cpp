#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  processes_.clear();
  for (auto pid : LinuxParser::Pids())
  {
    Process process(pid);
    processes_.push_back(process);
  }
  sort(processes_.rbegin(), processes_.rend()); 
  return processes_;
}


// TESTED: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TESTED: Return the system's memory utilization
float System::MemoryUtilization() {
  string line;
  string bla;
  string mem_total;
  string mem_free;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  if (stream.is_open()) {
    // Get info about MemTotal
    std::getline(stream, line);
    std::istringstream linestream1(line);
    linestream1 >> bla >> mem_total;
    // Get info about MemFree
    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> bla >> mem_free;
  }
  return (1.0 - (std::stof(mem_free) / std::stof(mem_total)));
}

// TESTED: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TESTED: Return the number of processes actively running on the system
int System::RunningProcesses() {
  string line;
  string s_running_proc = "";
  string n_running_proc;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (s_running_proc != "procs_running") {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> s_running_proc >> n_running_proc;
    }
  }
  return std::stoi(n_running_proc);
}

// TESTED: Return the total number of processes on the system
int System::TotalProcesses() {
  string line;
  string s_total_proc = "";
  string n_total_proc;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (s_total_proc != "processes") {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> s_total_proc >> n_total_proc;
    }
  }
  return std::stoi(n_total_proc);
}

// TESTED: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
