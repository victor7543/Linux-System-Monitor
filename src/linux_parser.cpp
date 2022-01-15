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
using std::stod;
using std::size_t;
using std::string;
using std::to_string;
using std::istringstream;
using std::vector;
using std::ifstream;
using std::getline;
using std::replace;
# if __has_include(<filesystem>)
    #include <filesystem>  
    namespace fs = std::filesystem;  
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
# endif

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os;
  string kernel;
  string version;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  const fs::path& directory = kProcDirectory; 
  if (fs::exists(directory) && fs::is_directory(directory)) {
    for (auto entry: fs::directory_iterator(directory)) {
      string dir = entry.path();
      size_t pos = kProcDirectory.size();
      string file = dir.substr(pos);
        if (std::all_of(file.begin(), file.end(), isdigit)) {
          int pid = stoi(file);
          pids.emplace_back(pid);
      }
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  int mem_total = -1;
  int mem_free = -1;
  int mem_utilized = 0;
  ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(getline(stream, line)) {
      if (line.find(filterMemTotalString) != string::npos) {
        int index = line.find(" ");
        mem_total = stoi(line.substr(index + 1));
      }
      if (line.find(filterMemFreeString) != string::npos) {
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

double LinuxParser::UpTime() { 
  string line;
  string str_uptime;
  double uptime = 0.0;
  ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> str_uptime;
    uptime = stod(str_uptime);
  }
  return uptime;
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_data = {};
  string line;
  string line_data;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(getline(stream, line)) {
      if (line.find(filterCpu) != string::npos) {
        int index = line.find(" ");
        line_data = line.substr(index + 1);
        istringstream ld_ss(line_data);
        string data;
        while (ld_ss >> data) {
          cpu_data.emplace_back(data);
        }
        return cpu_data;
      }
    }
  }
  return cpu_data;
}

vector<int> LinuxParser::ProcCpuUtilization(int pid) {
  string line;
  string value;
  vector<int> cpu_values;
  ifstream stream(kProcDirectory + to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    if (getline(stream, line)) {
      istringstream linestream(line);
      for (int i = 0; i < 22; i++) {
        linestream >> value;
        if ((i > 12 && i < 17)) {
          cpu_values.emplace_back(stoi(value));
        }
      }
      return cpu_values;
    }
  }
  return vector<int> {0,0,0,0};
}

int LinuxParser::TotalProcesses() {
  string line;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(getline(stream, line)) {
      if (line.find(filterProcesses) != string::npos) {
        int index = line.find(" ");
        return stoi(line.substr(index + 1));
      }
    }
  }
  return 0; 
}

int LinuxParser::RunningProcesses() {
  string line;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(getline(stream, line)) {
      if (line.find(filterRunningProcesses) != string::npos) {
        int index = line.find(" ");
        return stoi(line.substr(index + 1));
      }
    }
  }
  return 0;
  }

string LinuxParser::Command(int pid) {
  string line;
  ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    if (getline(stream, line)) {
      return line;
    }
  }
  return string();
  }

string LinuxParser::Ram(int pid) {
  string line;
  string ram_used;
  ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream linestream(line);
      if (line.find(filterProcMem) != string::npos) {         // using VmRSS instead of VmSize, because VmSize would be the sum of all the virtual memory being used, 
        linestream >> line >> ram_used;                       // while vmRSS is the exact physical memory being used as a part of Physical RAM. Source: https://man7.org/linux/man-pages/man5/proc.5.html
        return ram_used;
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid) {
  string line;
  string uid;
  ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream linestream(line);
      if (line.find(filterUID) != string::npos) {
        linestream >> line >> uid;
        return uid;
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string uid = Uid(pid), line, username, password, line_uid;
  ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      replace( line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      linestream >> username >> password >> line_uid;
      if (line_uid == uid) {
        return username;
      }
    }
  }
  return string();
}

int LinuxParser::ProcUpTime(int pid) {
  string line, uptime;
  ifstream stream(kProcDirectory + to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    if (getline(stream, line)) {
      istringstream linestream(line);
      for (int i = 0; i < 22; i++) {
        linestream >> uptime;
      }
      return stoi(uptime);
    }
  }
  return 0;
}
