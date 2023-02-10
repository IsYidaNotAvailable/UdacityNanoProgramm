#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() {
  return pid_;
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const {
  vector<std::string> values;
  std::string line, value;
  int i = 0;
  
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value && i < 23) {
      values.emplace_back(value);
      i++;
    }
  }
  // total_time
  float total_time = std::stof(values[13]) + std::stof(values[14]) + std::stof(values[15]) + std::stof(values[16]);
  // seconds
  float seconds = (1.0 * LinuxParser::UpTime(-1)) - (std::stof(values[21]) / sysconf(_SC_CLK_TCK));
  // cpu_usage
  return total_time / sysconf(_SC_CLK_TCK) / seconds;
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& other) const {
  float utilization_this = this->CpuUtilization();
  float utilization_other = other.CpuUtilization();
  return utilization_this < utilization_other;
}