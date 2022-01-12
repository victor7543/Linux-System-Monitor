#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <chrono>

class Process {
 public:
  Process(int pid) : pid_(pid) {}
  Process(int pid, Process* PrevProcess) : pid_(pid), PrevProcess_(PrevProcess) {}
  int Pid();                              
  std::string User();                     
  std::string Command();                  
  double CpuUtilization();                
  std::string Ram();                      
  long int UpTime();                      
  bool operator<(Process const& a) const; 

 private:
  int pid_ = 0, u_time = 0, s_time = 0, cu_time = 0, cs_time = 0, start_time = 0;
  double sys_uptime = 0.0;
  std::chrono::_V2::system_clock::time_point t_point;
  Process* PrevProcess_ = nullptr;
};

#endif