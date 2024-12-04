#pragma once

#ifndef PROCESS_LIST_HPP
#define PROCESS_LIST_HPP

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<vector>
#include<unistd.h>
#include<algorithm>

enum pstate_t {PENDING, QUEUED, EXECUTING, DONE};
enum process_type_t {FIFO, SJF, PRIO};

inline std::string showstate(pstate_t state){
    switch(state){
    case PENDING: return "PENDING";
    case QUEUED: return "QUEUED";
    case EXECUTING: return "EXECUTING";
    case DONE: return "DONE";
    default: return "UNDEFINED";
    }
}

typedef struct Process{
    int timestamp;
    int burst_duration;
    int priority;
    pstate_t state;
    int completion_time;
} process_t;

inline bool p_comp_burst(const process_t *p1, const process_t *p2){
    return (p1->burst_duration < p2->burst_duration);
}

inline bool p_comp_priority(const process_t *p1, const process_t *p2){
    return (p1->priority < p2->priority);
}

class ProcessList{
private:
    std::string column_names;
    int num_processes;
    std::vector<process_t> process_list_FIFO;
    std::vector<process_t> process_list_SJF;
    std::vector<process_t> process_list_PRIO;
public:
    ProcessList(std::ifstream& datafile);
    ~ProcessList();
    void display(process_type_t scheduler_type, int num_procs_to_display);
    void FIFO_scheduler();
    void SJF_scheduler();
    void PRIORITY_scheduler();
};

inline ProcessList::ProcessList(std::ifstream& datafile){
    std::getline(datafile, column_names);
    *(column_names.end()-1) = ' ';
    column_names += "\t Completion Times";
    process_t temp_process;
    while((datafile >> temp_process.timestamp) && (datafile >> temp_process.burst_duration) && (datafile >> temp_process.priority)){
        temp_process.state = PENDING;
        temp_process.completion_time = 0;
        process_list_FIFO.push_back(temp_process);
    }
    process_list_SJF = process_list_FIFO;
    process_list_PRIO = process_list_FIFO;
    num_processes = (process_list_FIFO.size() < 500) ? process_list_FIFO.size() : 500;
}

inline ProcessList::~ProcessList(){
}

inline void ProcessList::display(process_type_t scheduler_type, int num_procs_to_display=-1){
    if(num_procs_to_display == -1){
        num_procs_to_display = num_processes;
        std::cout<< "Displaying All Processes: \n" <<std::endl;
    }
    std::cout<< column_names <<std::endl;
    std::vector<process_t> processes;
    if(scheduler_type == FIFO)
        processes = process_list_FIFO;
    if(scheduler_type == SJF)
        processes = process_list_SJF;
    if(scheduler_type == PRIO)
        processes = process_list_PRIO;
    
    for(int i=0; i<num_procs_to_display; i++){
        std::cout<<std::setw(6)<<processes[i].timestamp<<"  \t\t"<<std::setw(4)<<processes[i].burst_duration<<"  \t\t"<<std::setw(3)<<processes[i].priority << "  \t\t\t"<<std::setw(6)<<processes[i].completion_time <<"\n";
    }
    std::cout<<std::endl;
}

inline void ProcessList::FIFO_scheduler(){
    bool isBusy = false;
    int current_time = 0;
    int completed_procs = 0;
    int next_proc_idx = 0;
    std::vector<process_t *> proc_queue;
    std::vector<process_t *>::iterator current_process;
    int offset;
    int burst = 0;
    while(completed_procs < num_processes){
        if(current_time == process_list_FIFO[next_proc_idx].timestamp && next_proc_idx < num_processes){
            proc_queue.push_back(&process_list_FIFO[next_proc_idx]);
            next_proc_idx++;
            continue;
        }
        if(!proc_queue.empty()){
            if(!isBusy){
                offset = 0;
                burst = (*(proc_queue.begin() + offset))->burst_duration;
            }
            current_process = proc_queue.begin() + offset;
            if((*current_process)->state == EXECUTING){
                (*current_process)->state = (--burst) ? EXECUTING: DONE;
            }
            else if((*current_process)->state == PENDING){
                (*current_process)->state = EXECUTING;
                isBusy = true;
            }
            if((*current_process)->state == DONE){
                (*current_process)->completion_time = current_time;
                proc_queue.erase(current_process);
                completed_procs++;
                isBusy = false;
                continue;
            }
        }
        current_time++;
    }
}

inline void ProcessList::SJF_scheduler(){
    bool isBusy = false;
    int current_time = 0;
    int completed_procs = 0;
    int next_proc_idx = 0;
    std::vector<process_t *> proc_queue;
    std::vector<process_t *>::iterator current_process;
    int offset;
    int burst = 0;
    while(completed_procs < num_processes){
        if(current_time == process_list_SJF[next_proc_idx].timestamp && next_proc_idx < num_processes){
            proc_queue.push_back(&process_list_SJF[next_proc_idx]);
            next_proc_idx++;
            continue;
        }
        if(!proc_queue.empty()){
            if(!isBusy){
                offset = std::min_element(proc_queue.begin(), proc_queue.end(), p_comp_burst) - proc_queue.begin();
                burst = (*(proc_queue.begin() + offset))->burst_duration;
            }
            current_process = proc_queue.begin() + offset;
            if((*current_process)->state == EXECUTING){
                (*current_process)->state = (--burst) ? EXECUTING: DONE;
            }
            else if((*current_process)->state == PENDING){
                (*current_process)->state = EXECUTING;
                isBusy = true;
            }
            if((*current_process)->state == DONE){
                (*current_process)->completion_time = current_time;
                proc_queue.erase(current_process);
                completed_procs++;
                isBusy = false;
                continue;
            }
        }
        current_time++;
    }
}

inline void ProcessList::PRIORITY_scheduler(){
    int current_time = 0;
    int completed_procs = 0;
    int next_proc_idx = 0;
    std::vector<process_t *> proc_queue;
    std::vector<process_t *>::iterator current_process;
    std::vector<int> burst_times;
    while(completed_procs < num_processes){
        if(current_time == process_list_PRIO[next_proc_idx].timestamp && next_proc_idx < num_processes){
            proc_queue.push_back(&process_list_PRIO[next_proc_idx]);
            burst_times.push_back(process_list_PRIO[next_proc_idx].burst_duration);
            next_proc_idx++;
            continue;
        }
        if(!proc_queue.empty()){
            int offset = std::max_element(proc_queue.begin(), proc_queue.end(), p_comp_priority) - proc_queue.begin();
            current_process = proc_queue.begin() + offset;
            if((*current_process)->state == EXECUTING){
                (*current_process)->state = (--burst_times[offset]) ? EXECUTING: DONE;
            }
            else if((*current_process)->state == PENDING){
                (*current_process)->state = EXECUTING;
            }
            if((*current_process)->state == DONE){
                (*current_process)->completion_time = current_time;
                proc_queue.erase(current_process);
                burst_times.erase(burst_times.begin()+offset);
                completed_procs++;
                continue;
            }
        }
        current_time++;
    }
}

#endif
