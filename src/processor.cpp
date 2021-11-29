#include "processor.h"
#include <iostream>

using namespace std;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int prev_idle, idle, prev_non_idle, non_idle, prev_total, total, total_dif, idle_dif;
    prev_idle = PrevCPU->idle + PrevCPU->iowait;
    idle = this->idle + this->iowait;
    prev_non_idle = PrevCPU->user + PrevCPU->nice + PrevCPU->system + PrevCPU->irq + PrevCPU->softirq + PrevCPU->steal;
    non_idle = this->user + this->nice + this->system + this->irq + this->softirq + this->steal;
    prev_total = prev_idle + prev_non_idle;
    total = idle + non_idle;
    total_dif = total - prev_total;
    idle_dif = idle - prev_idle;
    if (total_dif) {
        return (total_dif - idle_dif) / static_cast<double>(total_dif);
    }
    else return 0.0;

    /*auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
 
    cout << "Elapsed time in nanoseconds: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ns" << endl; */

}