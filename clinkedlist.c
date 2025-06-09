#include "/opt/localc/cheaders/crayon.h"
#include "/opt/localc/cheaders/assertf.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct _LinkedListNode LinkedListNode;
typedef struct _LinkedList LinkedList;

struct _LinkedListNode {
    void * Element;
    LinkedListNode * prev;
    LinkedListNode * next;
};

struct _LinkedList {
    LinkedListNode * Head;
    LinkedListNode * Tail;
    u64 Size;
};

static LinkedListNode * llnode_create(void * elem)
{
    LinkedListNode * llnode = new(LinkedListNode);
    scp(llnode);
    llnode->prev = nil;
    llnode->next = nil;
    llnode->Element = elem;
    return llnode;
}

LinkedList * ll_establish()
{
    LinkedList * list = new(LinkedList);
    scp(list);
    list->Head = nil;
    list->Tail = nil;
    list->Size = 0;
    return list;
}

LinkedList * ll_establish_with(void * elem)
{
    LinkedListNode * llnode = llnode_create(elem);
    LinkedList * list = new(LinkedList);
    scp(list);
    list->Head = llnode;
    list->Tail = llnode;
    list->Size = 1;
    return list;
}

void ll_insert_from_back(LinkedList * plist, void * elem)
{
    scp(plist);
    scp(elem);

    if (plist->Size == 0) {
        plist->Head = plist->Tail = llnode_create(elem);
    } else {
        LinkedListNode * llnode = llnode_create(elem);
        plist->Tail->next = llnode;
        plist->Tail = llnode;
    }
    plist->Size += 1;
}

void ll_insert_from_front(LinkedList * plist, void * elem)
{
    scp(plist);
    scp(elem);

    if (plist->Size == 0) {
        plist->Head = plist->Tail = llnode_create(elem);
    } else {
        LinkedListNode * llnode = llnode_create(elem);
        llnode->next = plist->Head;
        plist->Head = llnode;
    }
    plist->Size += 1;
}

static void llnode_free(LinkedListNode * llnode, void (*freefn)(void*))
{
    if (!llnode) return;
    if (!freefn) freefn = free;

    freefn(llnode->Element);
    free(llnode);
}

void ll_clear(LinkedList * plist, void (*freefn)(void*))
{
    if (!plist) return;
    if (plist->Size == 0) return;
    if (!freefn) freefn = free;

    LinkedListNode * tail = plist->Tail;
    LinkedListNode * curr = plist->Head;
    while (curr != tail) 
    {
    }
    plist->Size = 0;
}

void ll_easy_clear(LinkedList * plist)
{
    ll_clear(plist, nil);
}

void ll_free(LinkedList * plist, void (*freefn)(void*))
{
    if (!plist) return;
    ll_clear(plist, freefn);
    free(plist);
}

void ll_easy_free(LinkedList * plist)
{
    ll_free(plist, nil);
}

