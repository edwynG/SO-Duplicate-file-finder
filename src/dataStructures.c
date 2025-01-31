#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dataStructures.h"

// Implementación de dataStructures.h para más detalle vea dicho archivo

int isEmpty(struct List *list)
{
    return list->head == NULL;
};

int addNode(struct List *list, void *value)
{
    if (list == NULL)
        return 0;

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->value = value;
    newNode->next = NULL;
    newNode->before = NULL;
    list->size++;

    // Verifica si no hay ningun elemento
    if (list->head == NULL)
    {
        // Inserta el nuevo elemento
        list->head = newNode;
        list->tail = newNode;
        return 1;
    }

    // Inserta el nuevo elemento al final
    list->tail->next = newNode;
    newNode->before = list->tail;
    list->tail = newNode;
    return 1;
};

int removeNode(struct List *list, struct Node *node)
{
    if (list == NULL || node == NULL)
        return 0;

    if (node == list->head)
    {
        list->head = node->next;
        if (list->head != NULL)
        {
            list->head->before = NULL;
        }
        else
        {
            list->tail = NULL; // Si la lista queda vacía
        }

        free(node);
        return 1;
    }

    if (node == list->tail)
    {
        list->tail = node->before;
        list->tail->next = NULL;
        free(node);
        return 1;
    }

    node->before->next = node->next;
    node->next->before = node->before;
    free(node);
    list->size--;
    return 1;
};

void destructor(struct List *list)
{
    if (list == NULL)
        return;

    struct Node *temp = getHead(list);
    while (temp != NULL)
    {
        struct Node *t = temp->next;
        free(temp);
        temp = t;
        list->size--;
    }
    free(list);
};

struct Node *getHead(struct List *list)
{
    return list->head;
};

struct Node *getTail(struct List *list)
{
    return list->tail;
};

struct List *createList()
{
    struct List *list = (struct List *)malloc(sizeof(struct List));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
};

void setHashNode(struct Node* node, char* hash){
    if(node == NULL || hash == NULL){
        return;
    }
    node->hash = (char *)malloc(strlen(hash) + 1);
     strcpy(node->hash, hash);
};