#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "Queue.h"
#include "task.h"

void cpu(FILE* Simulation_Log, Queue* readyQueue, int cpuNum);

#endif
