#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

using std::string;
using std::vector;

namespace LinuxParser {
// Paths
const string kProcDirectory{"/proc/"};
const string kCmdlineFilename{"/cmdline"};
const string kCpuinfoFilename{"/cpuinfo"};
const string kStatusFilename{"/status"};
const string kStatFilename{"/stat"};
const string kUptimeFilename{"/uptime"};
const string kMeminfoFilename{"/meminfo"};
const string kVersionFilename{"/version"};
const string kOSPath{"/etc/os-release"};
const string kPasswordPath{"/etc/passwd"};
// Filter strings
const string filterProcesses("processes");
const string filterRunningProcesses("procs_running");
const string filterMemTotalString("MemTotal");
const string filterMemFreeString("MemFree");
const string filterCpu("cpu");
const string filterUID("Uid");
const string filterProcMem("VmRSS:");

// System
float MemoryUtilization();
double UpTime();
vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
string OperatingSystem();
string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
vector<string> CpuUtilization();
vector<int> ProcCpuUtilization(int pid);
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
string Command(int pid);
string Ram(int pid);
string Uid(int pid);
string User(int pid);
int ProcUpTime(int pid);
};  // namespace LinuxParser

#endif