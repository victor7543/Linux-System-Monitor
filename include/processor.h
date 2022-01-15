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
   user(stoi(data[0])),
   nice(stoi(data[1])),
   system(stoi(data[2])),
   idle(stoi(data[3])),
   iowait(stoi(data[4])),
   irq(stoi(data[5])), 
   softirq(stoi(data[6])),
   steal(stoi(data[7])),
   guest(stoi(data[8])),
   guest_nice(stoi(data[9])) {
  }
  Processor(vector<string> data, Processor* PrevCPU) : Processor(data) {
    PrevCPU_ = PrevCPU;
  }
  float Utilization();

 private:
  int user = 0;
  int nice = 0;
  int system = 0;
  int idle = 0;
  int iowait = 0;
  int irq = 0;
  int softirq = 0;
  int steal = 0;
  int guest = 0;
  int guest_nice = 0;
  Processor* PrevCPU_ = nullptr;
};

#endif