#include "LinkedList.h"

LinkedList* newLinkedList()
{
    LinkedList* ll = (LinkedList*) malloc(sizeof(LinkedListNode));
    ll->head = NULL;
    ll->tail = NULL;
    ll->numNode = 0;

    return ll;
}


void insertFirst(LinkedList* ll, void* data)
{
    LinkedListNode* node = (LinkedListNode*) malloc(sizeof(LinkedListNode));
    node->next = NULL;
    node->prev = NULL;
    node->data = data; 

    if(ll->head == NULL) /*List is empty*/
    {
        ll->head = node;
        ll->tail = node;
    }
    else /*List has atleast one item*/
    {
        node->next = ll->head;
        ll->head->prev = node;
        ll->head = node;
    }

    ll->numNode ++; /*Increase node counter*/
}

void insertLast(LinkedList* ll, void* data)
{
    LinkedListNode* node = (LinkedListNode*) malloc(sizeof(LinkedListNode));
    node->next = NULL;
    node->prev = NULL;
    node->data = data;

    if(ll->tail == NULL)
    {
        ll->head = node;
        ll->tail = node;
    } 
    else
    {
        node->prev = ll->tail;
        ll->tail->next = node;
        ll->tail = node;
    }

    ll->numNode ++; /*Increase node counter*/
}

void* removeFirst(LinkedList* ll)
{
    void* data;
    if(ll->head == NULL) 
    {
        data = NULL;
    }
    else
    {
        LinkedListNode* temp;
        temp = ll->head;

        data = temp->data;
        ll->head = temp->next;

        if(ll->head != NULL)
        {
            ll->head->prev = NULL;
        }
        else
        {
            ll->tail = NULL;
        }

       free(temp);
       ll->numNode --; /*decrease node counter*/
    }

    return data;
}

void* removeLast(LinkedList* ll)
{
    void* data;
    if(ll->tail == NULL) 
    {
        data = NULL;
    }
    else
    {
        LinkedListNode* temp;
        temp = ll->tail;

        data = temp->data;
        ll->tail = temp->prev;

        if(ll->tail != NULL)
        {
            ll->tail->next = NULL;
        }
        else
        {
            ll->head = NULL;
        }

       free(temp);
       ll->numNode --; /*decrease node counter*/
    }

    return data;
}

void* getNode(LinkedList* ll, int ii)
{
    LinkedListNode* curr;
    void* data;
    int i = 0;

    if(ll->numNode - 1 < ii)
    {
        data = NULL;
    }
    else
    {
        curr = ll->head;
        while(i < ii)
        {
            curr = curr->next;
            i++;
        }
        data = curr->data;
    }

    return data;
}


/*free the list*/
void softFreeList(LinkedList* ll)
{

    while(ll->head != NULL)
    {
        removeFirst(ll);
    }

    free(ll);
    ll = NULL;
}

void hardFreeList(LinkedList* ll, FreeMem fPtr)
{
    void* data;
    while(ll->head != NULL)
    {
        data = removeFirst(ll);
        (*fPtr)(data);
        data = NULL;
    }
    free(ll);
    ll = NULL;
}

