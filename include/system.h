#ifndef SYSTEM_H
#define SYSTEM_H

#include <optional>
#include <chrono>
#include "process.h"
#include "processor.h"

using std::optional;

class System {
 public:
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  std::vector<Process> processes = {}, prev_processes = {};
  Processor prev_cpu, cpu = Processor();
  int process_count = 0;
  vector<int> pids = {}, prev_pids = {};
};

#endif