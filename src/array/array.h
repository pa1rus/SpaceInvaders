#ifndef ARRAY_H
#define ARRAY_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

typedef struct
{
    int size;
    int capacity;
    int elem_size;
    void *data;
} Array;

void arrayInit(Array *arr, int capacity, int elem_size);
void *arrayAt(Array *arr, int index);
void arrayPush(Array *arr, void *elem);
void arraySwapRemove(Array *arr, int index);
void arrayFree(Array *arr);

#endif
