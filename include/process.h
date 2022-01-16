#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <memory>

using std::string;
using std::unique_ptr;
using std::make_unique;

class Process {
 public:
  Process(int pid) : pid_(pid) {}
  Process(int pid, Process &PrevProcess) : pid_(pid) {
    PrevProcess.PrevProcess_.reset();
    PrevProcess_ = make_unique<Process>(PrevProcess);
  }
  Process(Process& source) {
    pid_ = source.pid_;
    u_time = source.u_time;
    s_time = source.s_time;
    cu_time = source.cu_time;
    cs_time = source.cs_time;
    sys_uptime = source.sys_uptime;
    uptime = source.uptime;
    PrevProcess_ = std::move(source.PrevProcess_);
  }
  Process &operator=(Process &source) {
    if (this == &source) {
      return *this;
    }
    pid_ = source.pid_;
    u_time = source.u_time;
    s_time = source.s_time;
    cu_time = source.cu_time;
    cs_time = source.cs_time;
    sys_uptime = source.sys_uptime;
    uptime = source.uptime;
    PrevProcess_ = std::move(source.PrevProcess_);
    return *this;
  }
  Process(Process &&source) {
    pid_ = source.pid_;
    source.pid_ = 0;
    u_time = source.u_time;
    source.u_time = 0;
    s_time = source.s_time;
    source.s_time = 0;
    cu_time = source.cu_time;
    source.cu_time = 0;
    cs_time = source.cs_time;
    source.cs_time = 0;
    sys_uptime = source.sys_uptime;
    source.sys_uptime = 0;
    uptime = source.uptime;
    source.uptime = 0;
    PrevProcess_ = std::move(source.PrevProcess_);
  }
  Process &operator=(Process &&source) {
    if (this == &source) {
      return *this;
    }
     pid_ = source.pid_;
    source.pid_ = 0;
    u_time = source.u_time;
    source.u_time = 0;
    s_time = source.s_time;
    source.s_time = 0;
    cu_time = source.cu_time;
    source.cu_time = 0;
    cs_time = source.cs_time;
    source.cs_time = 0;
    sys_uptime = source.sys_uptime;
    source.sys_uptime = 0;
    uptime = source.uptime;
    source.uptime = 0;
    PrevProcess_ = std::move(source.PrevProcess_);
    return *this;
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
  unique_ptr<Process> PrevProcess_ = nullptr;
};

#endif