#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;

class Process {
 public:
  Process(int pid) : pid_(pid) {}
  Process(int pid, Process* PrevProcess) : pid_(pid) {
    PrevProcess_ = new Process(pid);
    if(PrevProcess->PrevProcess_ != nullptr) {
      delete PrevProcess->PrevProcess_;
    }
    *PrevProcess_ = *PrevProcess;
  }
  int Pid();                              
  string User();                     
  string Command();                  
  double CpuUtilization();                
  int Ram();                      
  int UpTime();                      
  bool operator<(Process const& a) const;
 private:
  int pid_ = 0;
  int u_time = 0;
  int s_time = 0;
  int cu_time = 0;
  int cs_time = 0;
  double sys_uptime = 0.0;
  double uptime = 0.0;
  Process* PrevProcess_ = nullptr;
};

#endif