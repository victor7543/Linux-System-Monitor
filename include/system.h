#ifndef SYSTEM_H
#define SYSTEM_H

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                 
  std::vector<Process>& Processes();
  float MemoryUtilization();        
  long UpTime();                    
  int TotalProcesses();             
  int RunningProcesses();           
  std::string Kernel();             
  std::string OperatingSystem();    

 private:
  std::vector<Process> processes = {}, prev_processes = {};
  Processor prev_cpu, cpu = Processor();
  int process_count = 0;
  vector<int> pids = {}, prev_pids = {};
};

#endif