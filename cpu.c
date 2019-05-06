#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "Queue.h"
#include "task.h"
#include "cpu.h"

void cpu(FILE* Simulation_Log, Queue* readyQueue, int cpuNum)
{
    static int num_tasks = 0;
    static double total_waiting_time = 0.0;
    static double total_turnaround_time = 0.0;

    double diffWait;
    double diffTurn;
    time_t cTime;
    time_t sTime;
    struct tm *cpu_sTime;
    struct tm *cpu_cTime;

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    Task* t = (Task*)malloc(sizeof(Task));

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_mutex_lock(&mutex);

    while(!(readyQueue->length > 0))
    {
        pthread_cond_wait(&cond, &mutex);
    }
    
    t = dequeue(readyQueue);
    sTime = time(NULL);
    diffWait = difftime(sTime, t->aTime);
    cpu_sTime = localtime(&sTime);

    fprintf(Simulation_Log, "\nStatistics for CPU-%d: \n", cpuNum);
    fprintf(Simulation_Log, "Task # %d\n", t->taskID);
    fprintf(Simulation_Log, "Arrival time: %d:%d:%d\n", t->arrivalTime->tm_hour, t->arrivalTime->tm_min, t->arrivalTime->tm_sec);
    fprintf(Simulation_Log, "Service time: %d:%d:%d\n\n", cpu_sTime->tm_hour, cpu_sTime->tm_min, cpu_sTime->tm_sec);

    sleep(t->cpuBurst);
    cTime = time(NULL);
    diffTurn = difftime(cTime, t->aTime);
    cpu_cTime = localtime(&cTime);
    t->completionTime = cpu_cTime;
    num_tasks = num_tasks + 1;

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);

    total_waiting_time = total_waiting_time + diffWait;
    total_turnaround_time = total_turnaround_time + diffTurn;

    fprintf(Simulation_Log, "\nStatistics for CPU-%d: \n", cpuNum);
    fprintf(Simulation_Log, "Task # %d\n", t->taskID);
    fprintf(Simulation_Log, "Arrival time: %d:%d:%d\n", t->arrivalTime->tm_hour, t->arrivalTime->tm_min, t->arrivalTime->tm_sec);
    fprintf(Simulation_Log, "Completion time: %d:%d:%d\n\n", t->completionTime->tm_hour, t->completionTime->tm_min, t->completionTime->tm_sec);
    free(t);
}
