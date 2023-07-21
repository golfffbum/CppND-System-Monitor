#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  long hours, minutes;
  
  hours = seconds / 3600;
  // get the remainder then convert to minutes
  minutes = (seconds % 3600) / 60;
  // get remaining seconds
  seconds = seconds % 60;

  // format and padd with zeros if necessary
  return std::to_string(hours) + ":" +
    	(minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
    	(seconds < 10 ? "0" : "") + std::to_string(seconds);
}