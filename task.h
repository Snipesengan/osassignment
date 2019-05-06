#ifndef TASK_H
#define TASK_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Queue.h"

typedef struct Task {
    int taskID;
    int cpuBurst;
    time_t aTime;
    struct tm * arrivalTime;
    struct tm * completionTime;
} Task;

void insertTask(Task *t, Queue *q, int m);
void arrive_smlog(Task* t, FILE* Simulation_Log);
void end_smlog(FILE* Simulation_Log, int num_tasks);
void task(FILE *Task_File, FILE* Simulation_Log, Queue *readyQueue, int m);

#endif
