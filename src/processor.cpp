#include "processor.h"
#include <iostream>

using namespace std;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int prev_idle, idle, prev_non_idle, non_idle, prev_total, total, total_dif, idle_dif;
    prev_idle = PrevCPU_->idle + PrevCPU_->iowait;
    idle = this->idle + this->iowait;
    prev_non_idle = PrevCPU_->user + PrevCPU_->nice + PrevCPU_->system + PrevCPU_->irq + PrevCPU_->softirq + PrevCPU_->steal;
    non_idle = this->user + this->nice + this->system + this->irq + this->softirq + this->steal;
    prev_total = prev_idle + prev_non_idle;
    if (prev_total) {
        total = idle + non_idle;
        total_dif = total - prev_total;
        idle_dif = idle - prev_idle;
        return (total_dif - idle_dif) / static_cast<double>(total_dif);
    }
    else return 0.0;

}