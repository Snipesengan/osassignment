#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/*#include "task.h"*/
/*#include "cpu.h"*/
#include "Queue.h"
#include "scheduler.h"

/*Queue *readyQueue;*/
/*FILE *Task_File;
FILE *Simulation_Log;
int m;*/

/*readyQueue = newQueue();*/

void insertTask(Task* t)
{
    time_t aTime;

    pthread_mutex_lock(&mutex);
    while(!(readyQueue->length < m))
    {
        pthread_cond_wait(&condOne, &mutex);       
    }
    aTime = time(NULL);
    t->aTime = aTime;
    t->arrivalTime = localtime(&aTime);
    enqueue(readyQueue, t);
    pthread_cond_signal(&condOne);
    pthread_mutex_unlock(&mutex);
}

void end_smlog()
{
    time_t endTime;
    struct tm *end_time;
    endTime = time(NULL);
    end_time = localtime(&endTime);

    fprintf(Simulation_Log, "\nNo. of Tasks put in Ready_Queue: %d\n", num_tasks);
    /*fprintf(Simulation_Log, "Finished at Time: %s\n", asctime(end_time));*/
    fprintf(Simulation_Log, "Finished at Time: %d:%d:%d\n", end_time->tm_hour, end_time->tm_min, end_time->tm_sec);
}

void arrive_smlog(Task* t)
{
    time_t aTime;
    struct tm* a_Time;
    aTime = time(NULL);
    a_Time = localtime(&aTime);

    fprintf(Simulation_Log, "%d:%d\n", t->taskID, t->cpuBurst);
    /*fprintf(Simulation_Log, "Arrival Time: %s\n", asctime(a_Time));*/
    fprintf(Simulation_Log, "Arrival Time: %d:%d:%d\n", a_Time->tm_hour, a_Time->tm_min, a_Time->tm_sec);
}

int main(int argc, char *argv[])
{
    pthread_t task_threadID;
    pthread_t CPU1_threadID;
    pthread_t CPU2_threadID;
    pthread_t CPU3_threadID;

    void* cpu1;
    void* cpu2;
    void* cpu3;

    pthread_cond_init(&condOne, NULL);
    pthread_cond_init(&condTwo, NULL);

    cpu1 = malloc(sizeof(int));
    cpu2 = malloc(sizeof(int));
    cpu3 = malloc(sizeof(int));
    /*int m;*/
    if(argc >=3 || argc == 1)
    {
        printf("Incorrect command line arguments. Enter a size for ready queue \n");
        scanf("%d", &m);
    }
    else
    {
        m = atoi(argv[1]);
    }

    readyQueue = newQueue();

    /*Queue *readyQueue;
    readyQueue = newQueue();
    
    FILE *Task_File;
    FILE *Simulation_Log;*/

    Task_File = fopen("task_file", "r");
    Simulation_Log = fopen("simulation_log", "w");
    /*FILE IO ERROR CHECKING FOR CAMMY KERR*/

    *((int*)cpu1) = 1;
    *((int*)cpu2) = 2;
    *((int*)cpu3) = 3;

    pthread_create(&task_threadID, NULL, &taskThread, NULL);
    pthread_create(&CPU1_threadID, NULL, &CPUThread, cpu1);
    pthread_create(&CPU2_threadID, NULL, &CPUThread, cpu2);
    pthread_create(&CPU3_threadID, NULL, &CPUThread, cpu3);

    pthread_join(task_threadID, NULL);
    pthread_join(CPU1_threadID, NULL);
    pthread_join(CPU2_threadID, NULL);
    pthread_join(CPU3_threadID, NULL);

    pthread_mutex_destroy(&mutex);

    free(cpu1);
    free(cpu2);
    free(cpu3);
    softFreeQueue(readyQueue);
    fclose(Task_File);
    fclose(Simulation_Log);
    return 0;
}

void* taskThread(void* args) /*Task Thread*/
{
    while (!feof(Task_File))/*must include header file of scheduler so that we can define the file there*/
    {
        Task* taskOne = (Task*)malloc(sizeof(Task));
        Task* taskTwo = (Task*)malloc(sizeof(Task));
        
        int tempID;
        int tempBurst;

        fscanf(Task_File, "%d %d", &tempID, &tempBurst);

        taskOne->taskID = tempID;
        taskOne->cpuBurst = tempBurst;
        
        insertTask(taskOne);
        num_tasks = num_tasks + 1;
        arrive_smlog(taskOne);

        fscanf(Task_File, "%d %d", &tempID, &tempBurst);

        taskTwo->taskID = tempID;
        taskTwo->cpuBurst = tempBurst;

        insertTask(taskTwo);
        num_tasks = num_tasks + 1;
        arrive_smlog(taskTwo);

        free(taskOne);
        free(taskTwo); 
    }
    end_smlog();
    return 0;
}

void* CPUThread(void* args)
{
    int cpuNum = *((int*) args);
    double diffWait;
    double diffTurn;

    time_t cTime;
    time_t sTime;
    struct tm *cpu_sTime;
    struct tm *cpu_cTime;

    Task* t = (Task*)malloc(sizeof(Task));
    while(1){
        pthread_mutex_lock(&mutex);

        while(!(readyQueue->length > 0))
        {
            pthread_cond_wait(&condTwo, &mutex);
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

	pthread_cond_signal(&condTwo);
	pthread_mutex_unlock(&mutex);

	total_waiting_time = total_waiting_time + diffWait;
	total_turnaround_time = total_turnaround_time + diffTurn;

	fprintf(Simulation_Log, "\nStatistics for CPU-%d: \n", cpuNum);
	fprintf(Simulation_Log, "Task # %d\n", t->taskID);
	fprintf(Simulation_Log, "Arrival time: %d:%d:%d\n", t->arrivalTime->tm_hour, t->arrivalTime->tm_min, t->arrivalTime->tm_sec);
	fprintf(Simulation_Log, "Completion time: %d:%d:%d\n\n", t->completionTime->tm_hour, t->completionTime->tm_min, t->completionTime->tm_sec);
	free(t);
    }
}







