#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
namespace lp = LinuxParser;

Processor& System::Cpu() {
    vector<string> cpu_data = lp::CpuUtilization();
    prev_cpu = cpu;
    cpu = Processor(cpu_data, &prev_cpu);
    return cpu;
}

vector<Process> &System::Processes() {
    prev_processes = processes;
    processes = vector<Process> {};
    prev_pids = pids;
    pids = lp::Pids();
    for (int pid : pids) {
        bool pid_found = false;
        for (Process proc : prev_processes) {
            if (pid == proc.Pid()) {
                Process *p = new Process(pid);
                *p = proc;
                processes.push_back(Process(pid, p));
                pid_found = true;
                break;
            }
        }
        if (!pid_found) {
            processes.push_back(Process(pid));
        }
    }
    return processes;
}

std::string System::Kernel() {
    return lp::Kernel();
}

float System::MemoryUtilization() {
    return lp::MemoryUtilization();
}

std::string System::OperatingSystem() {
    return lp::OperatingSystem();
}

int System::RunningProcesses() {
    return lp::RunningProcesses();
}

int System::TotalProcesses() { 
    return lp::TotalProcesses();
}

long int System::UpTime() {
    return lp::UpTime();
}
