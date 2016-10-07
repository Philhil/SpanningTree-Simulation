#ifndef SPANNINGTREE_SIMULATION_LIST_H
#define SPANNINGTREE_SIMULATION_LIST_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef void (*freeFunction)(void *);//pointer to function for free malloc'd objects. It will be called to free data when elem in List will be destroyed
typedef bool (*listIterator)(void *);//pointer to function for foreatch iteration. If return FALSE iteration will stop
typedef bool (*listCompare)(void *, void *);//pointer to function for compare. If return true, enty exist and iteration will stop and return true.

typedef struct _listNode
{
    void *data;
    struct _listNode *next;
} listNode;

typedef struct
{
    int logicalLength;
    int elementSize;
    listNode *head;
    listNode *tail;
    freeFunction freeFn;
} list;

void list_new(list *list, int elementSize, freeFunction freeFn);
void list_destroy(list *list);

void list_prepend(list *list, void *element);
void list_append(list *list, void *element);
int list_size(list *list);

void list_for_each(list *list, listIterator iterator);
bool list_exist(list *list, void *comparevalue, listCompare compare);
void list_head(list *list, void *element, bool removeFromList); //second param: get data of element data. third param: remove from List
void list_tail(list *list, void *element);


#endif //SPANNINGTREE_SIMULATION_LIST_H