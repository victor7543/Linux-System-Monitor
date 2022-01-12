#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <chrono>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) {}
  Process(int pid, Process* PrevProcess) : pid_(pid), PrevProcess_(PrevProcess) {}
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  double CpuUtilization();                 // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_ = 0, u_time = 0, s_time = 0, cu_time = 0, cs_time = 0, start_time = 0;
  double sys_uptime = 0.0;
  std::chrono::_V2::system_clock::time_point t_point;
  Process* PrevProcess_ = nullptr;
};

#endif