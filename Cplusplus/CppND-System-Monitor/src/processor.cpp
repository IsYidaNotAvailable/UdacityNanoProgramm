#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  // utilization = active_jiffies / total_jiffies;
  float total_jiffies = 1.0 * LinuxParser::Jiffies();
  float active_jiffies = 1.0 * LinuxParser::ActiveJiffies();
  return active_jiffies / total_jiffies;
}