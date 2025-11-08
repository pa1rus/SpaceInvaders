#include "array.h"

void arrayInit(Array *arr, int capacity, int elem_size)
{
    arr->size = 0;
    arr->capacity = capacity;
    arr->elem_size = elem_size;
    arr->data = malloc(capacity * elem_size);
    if (!arr->data)
    {
        printf("Malloc failed\n");
        exit(1);
    }
}

void *arrayAt(Array *arr, int index)
{
    if (index < 0 || index >= arr->size)
    {
        exit(1);
    }
    return (char *)arr->data + index * arr->elem_size;
}

void arrayPush(Array *arr, void *elem)
{

    if (arr->size == arr->capacity)
    {

        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * arr->elem_size);

        if (!arr->data)
        {

            printf("Realloc failed\n");
            exit(1);
        }
    }

    memcpy((char *)arr->data + arr->size * arr->elem_size, elem, arr->elem_size);
    arr->size++;
}

void arraySwapRemove(Array *arr, int index)
{

    if (index < 0 || index >= arr->size)
        return;

    int last = arr->size - 1;

    memcpy(
        (char *)arr->data + index * arr->elem_size,
        (char *)arr->data + last * arr->elem_size,
        arr->elem_size);

    arr->size--;
}

void arrayFree(Array *arr)
{

    free(arr->data);
}
