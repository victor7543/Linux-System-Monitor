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

// TODO: Return this process's ID
int Process::Pid() {
    return this->pid_; 
}

// TODO: Return this process's CPU utilization
double Process::CpuUtilization() {
    if (PrevProcess_) {
        vector<int> cpu_values = lp::ProcCpuUtilization(pid_);
        this->sys_uptime = lp::UpTime();
        this->u_time = cpu_values[0], this->s_time = cpu_values[1], this->cu_time = cpu_values[2], this->cs_time = cpu_values[3], this->start_time = lp::ProcUpTime(this->Pid());
        int total_time = u_time + s_time;
        int prev_total_time = PrevProcess_->u_time + PrevProcess_->s_time;
        total_time += this->cu_time + this->cs_time;
        prev_total_time += PrevProcess_->cu_time + PrevProcess_->cs_time;
        double seconds = this->sys_uptime - (this->start_time / static_cast<double>(CLOCKS_PER_SEC));
        double prev_seconds = PrevProcess_->sys_uptime - (this->start_time / static_cast<double>(CLOCKS_PER_SEC));
        double seconds_diff = seconds - prev_seconds;
        double time_diff = total_time - prev_total_time;
        time_diff = time_diff / static_cast<double>(sysconf(_SC_CLK_TCK));
        double utilization = time_diff / seconds_diff;
        return utilization;
    }
    else return 0.0;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return lp::Command(pid_);
}

// TODO: Return this process's memory utilization
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

// TODO: Return the user (name) that generated this process
string Process::User() {
    return lp::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    long clock_ticks = lp::ProcUpTime(pid_);
    long seconds = clock_ticks / sysconf(_SC_CLK_TCK);
    this->start_time = seconds;
    return seconds;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }