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

typedef struct Process{
    int timestamp;
    int burst_duration;
    int priority;
    pstate_t state;
    int completion_time;
} process_t;


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

#endif
