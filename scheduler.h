#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Queue.h"
/*#include "task.h"
#include "cpu.h"*/

Queue *readyQueue;
/*readyQueue = newQueue();*/
FILE *Task_File;
FILE *Simulation_Log;
int m;
int num_tasks = 0;
double total_waiting_time = 0.0;
double total_turnaround_time = 0.0;
pthread_mutex_t mutex;
pthread_cond_t condOne;
pthread_cond_t condTwo;

typedef struct Task {
    int taskID;
    int cpuBurst;
    time_t aTime;
    struct tm * arrivalTime;
    struct tm * completionTime;
} Task;

void* taskThread(void* args);
void* CPUThread(void* args);
void insertTask(Task* t);
void end_smlog();
void arrive_smlog(Task* t);

#endif
