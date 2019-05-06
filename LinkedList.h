#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <string.h>

typedef void (*FreeMem)(void*);

typedef struct LinkedListNode {
    void* data;
    struct LinkedListNode* next; /*struct pointer*/
    struct LinkedListNode* prev; /*struct pointer*/

} LinkedListNode;

typedef struct LinkedList{
	LinkedListNode* head;
	LinkedListNode* tail;
	int numNode;
} LinkedList;

LinkedList* newLinkedList();
void insertFirst(LinkedList* ll, void* data);
void insertLast(LinkedList* ll, void* data);
void* removeFirst(LinkedList* ll);
void* removeLast(LinkedList* ll);
void* getNode(LinkedList* ll, int ii);
void printList(LinkedList* ll);
void softFreeList(LinkedList* ll);
void hardFreeList(LinkedList* ll, FreeMem fnc);

#endif
