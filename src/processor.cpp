#include "processor.h"
#include <iostream>

float Processor::Utilization() {
    int prev_idle;
    int idle;
    int prev_non_idle;
    int non_idle;
    int prev_total;
    int total;
    int total_dif;
    int idle_dif;
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