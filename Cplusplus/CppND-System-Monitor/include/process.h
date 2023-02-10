#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) {
    pid_ = pid;
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);
  };

  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // DONE: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
};

#endif