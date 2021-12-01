#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

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
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() {
    return lp::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    double ram;
    int pos;
    string ram_str = lp::Ram(pid_);
    ram = std::stod(ram_str)/ 1000.0f;
    ram_str = to_string(ram);
    pos = ram_str.find(".");
    ram_str = ram_str.substr(0, pos+2);
    return ram_str;
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return lp::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    long clock_ticks = lp::UpTime(pid_);
    long seconds = clock_ticks / sysconf(_SC_CLK_TCK);
    return seconds;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }