#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TESTED: Complete this helper function
string Format::ElapsedTime(long total_seconds) {
  int h = total_seconds / 3600;
  int m = (total_seconds % 3600) / 60;
  int s = total_seconds % 60;
  std::string formated_s = s > 9 ? std::to_string(s) : "0" + std::to_string(s);
  std::string formated_m = m > 9 ? std::to_string(m) : "0" + std::to_string(m);
  std::string formated_h = h > 9 ? std::to_string(h) : "0" + std::to_string(h);
  return formated_h + ":" + formated_m + ":" + formated_s;
}