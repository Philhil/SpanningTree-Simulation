#include "list.h"

//create new list
void list_new(list *list, int elementSize, freeFunction freeFn)
{
    assert(elementSize > 0);
    list->logicalLength = 0;
    list->elementSize = elementSize;
    list->head = list->tail = NULL;
    list->freeFn = freeFn;
}

//destroy complete list
void list_destroy(list *list)
{
    listNode *current;
    while(list->head != NULL)
    {
        current = list->head;
        list->head = current->next;
        list->logicalLength--;

        if(list->freeFn)
        {
            list->freeFn(current->data);
        }

        free(current->data);
        free(current);
    }
}

//add new elem at head
void list_prepend(list *list, void *element)
{
    listNode *node = malloc(sizeof(listNode));
    node->data = malloc(list->elementSize);
    memcpy(node->data, element, list->elementSize);

    node->next = list->head;
    list->head = node;

    // first node?
    if(!list->tail)
    {
        list->tail = list->head;
    }

    list->logicalLength++;
}

//add new elem at end
void list_append(list *list, void *element)
{
    listNode *node = malloc(sizeof(listNode));
    node->data = malloc(list->elementSize);
    node->next = NULL;

    memcpy(node->data, element, list->elementSize);

    if(list->logicalLength == 0)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }

    list->logicalLength++;
}

//iterate through list with iterator function
void list_for_each(list *list, listIterator iterator)
{
    assert(iterator != NULL);

    listNode *node = list->head;
    bool result = true;
    while(node != NULL && result)
    {
        result = iterator(node->data);
        node = node->next;
    }
}

//check if any elem has compare value by compare function
bool list_exist(list *list, void *comparevalue, listCompare compare)
{
    assert(compare != NULL);

    listNode *node = list->head;
    bool result = false;
    while(node != NULL && !result)
    {
        result = compare(comparevalue, node->data);
        node = node->next;
    }

    return result;
}

//get head elem and could be delete
void list_head(list *list, void *element, bool removeFromList)
{
    assert(list->head != NULL);

    listNode *node = list->head;
    memcpy(element, node->data, list->elementSize);

    if(removeFromList)
    {
        list->head = node->next;
        list->logicalLength--;

        if(list->freeFn)
        {
            list->freeFn(node->data);
        }

        free(node->data);
        free(node);
    }
}

//get last elem
void list_tail(list *list, void *element)
{
    assert(list->tail != NULL);
    listNode *node = list->tail;
    memcpy(element, node->data, list->elementSize);
}

//get size of list
int list_size(list *list)
{
    return list->logicalLength;
}