#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "task.h"
#include "Queue.h"

void insertTask(Task *t, Queue *q, int m)
{
    time_t aTime;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_mutex_lock(&mutex);
    while(!(q->length < m))
    {
        pthread_cond_wait(&cond, &mutex);       
    }
   
    /**currentTime = time(&aTime);*/
    /*time(&aTime);*/
    aTime = time(NULL);
    t->aTime = aTime;
    t->arrivalTime = localtime(&aTime);
    enqueue(q, t);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    /*free(&mutex);*/
    pthread_mutex_destroy(&mutex);
}

void arrive_smlog(Task* t, FILE* Simulation_Log)
{
    time_t aTime;
    struct tm* a_Time;
    aTime = time(NULL);
    a_Time = localtime(&aTime);

    fprintf(Simulation_Log, "%d:%d\n", t->taskID, t->cpuBurst);
    /*fprintf(Simulation_Log, "Arrival Time: %s\n", asctime(a_Time));*/
    fprintf(Simulation_Log, "Arrival Time: %d:%d:%d\n", a_Time->tm_hour, a_Time->tm_min, a_Time->tm_sec);
}

void end_smlog(FILE* Simulation_Log, int num_tasks)
{
    time_t endTime;
    struct tm *end_time;
    endTime = time(NULL);
    end_time = localtime(&endTime);

    fprintf(Simulation_Log, "\nNo. of Tasks put in Ready_Queue: %d\n", num_tasks);
    /*fprintf(Simulation_Log, "Finished at Time: %s\n", asctime(end_time));*/
    fprintf(Simulation_Log, "Finished at Time: %d:%d:%d\n", end_time->tm_hour, end_time->tm_min, end_time->tm_sec);
}

void task(FILE *Task_File, FILE *Simulation_Log, Queue *readyQueue, int m)
{
    static int num_tasks = 0;
    while (!feof(Task_File))/*must include header file of scheduler so that we can define the file there*/
    {
        Task* taskOne = (Task*)malloc(sizeof(Task));
        Task* taskTwo = (Task*)malloc(sizeof(Task));
        
        int tempID;
        int tempBurst;

        fscanf(Task_File, "%d %d", &tempID, &tempBurst);

        taskOne->taskID = tempID;
        taskOne->cpuBurst = tempBurst;
        
        insertTask(taskOne, readyQueue, m);
        num_tasks = num_tasks + 1;
        arrive_smlog(taskOne, Simulation_Log);

        fscanf(Task_File, "%d %d", &tempID, &tempBurst);

        taskTwo->taskID = tempID;
        taskTwo->cpuBurst = tempBurst;

        insertTask(taskTwo, readyQueue, m);
        num_tasks = num_tasks + 1;
        arrive_smlog(taskTwo, Simulation_Log);

        free(taskOne);
        free(taskTwo); 
    }
    end_smlog(Simulation_Log, num_tasks);
}










