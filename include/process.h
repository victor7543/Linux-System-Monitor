#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <chrono>

using std::string;

class Process {
 public:
  Process(int pid) : pid_(pid) {}
  Process(int pid, Process* PrevProcess) : pid_(pid), PrevProcess_(PrevProcess) {}
  int Pid();                              
  string User();                     
  string Command();                  
  double CpuUtilization();                
  int Ram();                      
  int UpTime();                      
  bool operator<(Process const& a) const; 

 private:
  int pid_ = 0, u_time = 0, s_time = 0, cu_time = 0, cs_time = 0;
  double sys_uptime = 0.0, uptime = 0.0;
  std::chrono::_V2::system_clock::time_point t_point;
  Process* PrevProcess_ = nullptr;
};

#endif