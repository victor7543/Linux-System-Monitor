#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
namespace lp = LinuxParser;

int Process::Pid() {
    return this->pid_; 
}

double Process::CpuUtilization() {
    if (PrevProcess_) {
        vector<int> cpu_values = lp::ProcCpuUtilization(pid_);
        this->u_time = cpu_values[0];
        this->s_time = cpu_values[1];
        this->cu_time = cpu_values[2];
        this->cs_time = cpu_values[3];
        this->UpTime();                 // making sure the uptime variable is updated
        int cpu_time = u_time + s_time;
        int prev_cpu_time = PrevProcess_->u_time + PrevProcess_->s_time;
        cpu_time += this->cu_time + this->cs_time;
        prev_cpu_time += PrevProcess_->cu_time + PrevProcess_->cs_time;
        double uptime_diff = this->uptime - PrevProcess_->uptime;
        double cpu_time_diff = cpu_time - prev_cpu_time;
        cpu_time_diff = cpu_time_diff / static_cast<double>(sysconf(_SC_CLK_TCK));
        double utilization = cpu_time_diff / uptime_diff;
        return utilization;
    }
    else return 0.0;
}

string Process::Command() {
    string command = lp::Command(pid_);
    if (command.size() > 50) {
        string trimmed_command = command.substr(0, 50);
        return trimmed_command.append("...");
    }
    return command;
}

string Process::Ram() {
    double ram;
    int pos;
    string ram_str = lp::Ram(pid_);
    if (ram_str.size() > 0) {
        ram = std::stod(ram_str)/ 1000.0f;
        ram_str = to_string(ram);
        pos = ram_str.find(".");
        ram_str = ram_str.substr(0, pos+2);
        return ram_str;
    }
    else {
        return "";
    }
}

string Process::User() {
    return lp::User(pid_);
}

int Process::UpTime() {
    int clock_ticks = lp::ProcUpTime(this->pid_);
    double start_time = clock_ticks / static_cast<double>(sysconf(_SC_CLK_TCK));
    this->sys_uptime = lp::UpTime();
    this->uptime = this->sys_uptime - start_time;
    return static_cast<int>(this->uptime);
}
