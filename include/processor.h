#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Processor {
 public:
  Processor() {}
  Processor(vector<string> data) : 
   user(stoi(data[0])), nice(stoi(data[1])),
   system(stoi(data[2])), idle(stoi(data[3])),
   iowait(stoi(data[4])), irq(stoi(data[5])), 
   softirq(stoi(data[6])), steal(stoi(data[7])),
   guest(stoi(data[8])), guest_nice(stoi(data[9])) {
  }
  Processor(vector<string> data, Processor* cpu) : Processor(data) {
    PrevCPU = cpu;
  }
  float Utilization();  // TODO: See src/processor.cpp

  Processor* PrevCPU = nullptr;
  // TODO: Declare any necessary private members
 private:
  int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
};

#endif