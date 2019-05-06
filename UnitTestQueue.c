#include "Queue.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    Queue* q = newQueue();
    int i;

    for(i = 0; i < 10; i++)
    {
        enqueue(q,(void*) &i);
    }

    
    for(i = 0; i < 10; i++)
    {
        int* dat;
        dat = (float*) dequeue(q);
        printf("%d %d\n",*dat,q->length);
    }

    dequeue(q);

    softFreeQueue(q);


     return 0;
}
