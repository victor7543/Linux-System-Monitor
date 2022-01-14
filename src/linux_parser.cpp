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
          pids.emplace_back(pid);
      }
    }
  }
  return pids;
}

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

double LinuxParser::UpTime() { 
  string line, str_uptime;
  double uptime = 0.0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> str_uptime;
    uptime = stod(str_uptime);
  }
  return uptime;
}

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
          cpu_data.emplace_back(data);
        }
        return cpu_data;
      }
    }
  }
  return cpu_data;
}

vector<int> LinuxParser::ProcCpuUtilization(int pid) {
  string line, value;
  vector<int> cpu_values;
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
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

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      return line;
    }
  }
  return string();
  }

string LinuxParser::Ram(int pid) {
  string line, vmsize_descr = "VmSize:", ram_used;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      if (line.find(vmsize_descr) != string::npos) {
        linestream >> vmsize_descr >> ram_used;
        return ram_used;
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid) {
  string line, uid_descr = "Uid", uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      if (line.find(uid_descr) != string::npos) {
        linestream >> uid_descr >> uid;
        return uid;
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string uid = Uid(pid), line, username, password, line_uid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace( line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
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
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i < 22; i++) {
        linestream >> uptime;
      }
      return std::stoi(uptime);
    }
  }
  return 0;
}
