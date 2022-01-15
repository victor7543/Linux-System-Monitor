#ifndef SYSTEM_H
#define SYSTEM_H

#include "process.h"
#include "processor.h"

using std::vector;
using std::string;

class System {
 public:
  Processor& Cpu();                 
  vector<Process>& Processes();
  float MemoryUtilization();        
  long UpTime();                    
  int TotalProcesses();             
  int RunningProcesses();           
  string Kernel();             
  string OperatingSystem();    

 private:
  vector<Process> processes = {};
  vector<Process> prev_processes = {};
  vector<int> pids = {}, prev_pids = {};
  int process_count = 0;
  Processor prev_cpu;
  Processor cpu = Processor();
};

#endif