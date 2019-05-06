#include "Queue.h"

Queue* newQueue()
{
    Queue* q = (Queue*) malloc(sizeof(Queue));
    q->ll    = newLinkedList();
    q->length = 0;
    return q;
}

void enqueue(Queue* q, void* data)
{
    insertLast(q->ll, data);
    q->length ++; 

}

void* dequeue(Queue* q)
{
    void* data = removeFirst(q->ll);
    q->length --;
    return data;
}

void softFreeQueue(Queue* q)
{
    softFreeList(q->ll);
    free(q);
}

void hardFreeQueue(Queue* q, FreeMem fnc)
{
    hardFreeList(q->ll,fnc);
    free(q);
}
