#ifndef GROWING_ARRAY_H
#define GROWING_ARRAY_H

#include <stdlib.h>

#define INTERVAL 4

typedef struct {
    int size;
    int capacity;
    int data_size;
    void* elements;
} GrowingArray;

void growing_array_init(GrowingArray *ga, int starting_capacity, int data_size);
void* at(GrowingArray* ga, int index);
void push(GrowingArray* ga, void *elem);
void pop(GrowingArray* ga);
void free_array(GrowingArray *ga);

#endif

