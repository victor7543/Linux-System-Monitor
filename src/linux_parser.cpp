#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::istringstream;
using std::vector;
namespace fs = std::filesystem;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  const fs::path& directory = kProcDirectory; 
  if (fs::exists(directory) && fs::is_directory(directory)) {
    for (auto entry: fs::directory_iterator(directory)) {
      string dir = entry.path();
      std::size_t pos = kProcDirectory.size();
      std::string file = dir.substr(pos);
        if (std::all_of(file.begin(), file.end(), isdigit)) {
          int pid = stoi(file);
          pids.push_back(pid);
      }
    }
  }
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, str_mem_total {"MemTotal"}, str_mem_free {"MemFree"};
  int mem_total {-1}, mem_free {-1}, mem_utilized;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.find(str_mem_total) != string::npos) {
        int index = line.find(" ");
        mem_total = stoi(line.substr(index + 1));
      }
      if (line.find(str_mem_free) != string::npos) {
        int index = line.find(" ");
        mem_free = stoi(line.substr(index + 1));
      }
      if (mem_total != -1 && mem_free != -1) {
        mem_utilized = mem_total - mem_free;
        return static_cast<double>(mem_utilized) / static_cast<double>(mem_total);
      }
    }
  }
  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, str_uptime;
  long uptime = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> str_uptime;
    uptime = stoi(str_uptime);
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_data = {};
  string line, cpu = "cpu", line_data;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.find(cpu) != string::npos) {
        int index = line.find(" ");
        line_data = line.substr(index + 1);
        istringstream ld_ss(line_data);
        string data;
        while (ld_ss >> data) {
          cpu_data.push_back(data);
        }
        return cpu_data;
      }
    }
  }
  return cpu_data;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string processes = "processes";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.find(processes) != string::npos) {
        int index = line.find(" ");
        return stoi(line.substr(index + 1));
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string processes = "procs_running";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      if (line.find(processes) != string::npos) {
        int index = line.find(" ");
        return stoi(line.substr(index + 1));
      }
    }
  }
  return 0;
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
