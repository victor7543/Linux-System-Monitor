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
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() {
    vector<string> cpu_data = lp::CpuUtilization();
    prev_cpu = cpu;
    cpu = Processor(cpu_data, &prev_cpu);
    return cpu;
}

// TODO: Return a container composed of the system's processes
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

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    return lp::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    return lp::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    return lp::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
    return lp::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    return lp::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    return lp::UpTime();
}
