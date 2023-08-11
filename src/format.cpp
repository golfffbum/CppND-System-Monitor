#include <string>
#include <iomanip>
#include <sstream>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  int h = seconds / 3600;
  int m = (seconds % 3600) / 60; // get the remainder then convert to minutes
  int s = (seconds % 3600) % 60; // get remaining seconds

  // setw, setfill to ensure h,m,s are always represented with 2 digits
  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << h << ":"
      << std::setw(2) << std::setfill('0') << m << ":"
      << std::setw(2) << std::setfill('0') << s;

  return oss.str();
}
