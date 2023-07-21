#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();

  // private members
 private:
    long prevTotal = 0;
    long prevIdle = 0;
};

#endif