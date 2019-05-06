#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

typedef struct Queue{
    LinkedList* ll;
    int length;
} Queue;

Queue* newQueue();
void enqueue(Queue* q, void* data);
void* dequeue(Queue* q);
void softFreeQueue(Queue* q);
void hardFreeQueue(Queue* q, FreeMem fnc);

#endif
