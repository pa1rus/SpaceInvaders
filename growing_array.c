#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define INTERVAL 4
//this is meant to be included in some other program

typedef struct {
    int size;
    int capacity;
    int data_size;
    void* elements;
} growing_array;

//init the array
growing_array growing_array_init(int starting_capacity,int data_size){
    growing_array ga;
    ga.size = 0;
    ga.capacity = starting_capacity;
    ga.data_size = data_size;
    ga.elements = malloc(ga.data_size * starting_capacity);
    return ga;
}

void* at(growing_array* ga, int index){
    if (index < ga->size){
        return (char*)ga->elements + index * ga->data_size;
    } else {
        printf("out of bounds array acess");
        exit(1);
    }
}

void push(growing_array* ga, void *elem){
    int index = ga->size; 
    if (ga->size == ga->capacity){
        ga->capacity += INTERVAL;
        ga->elements = realloc(ga->elements, ga->capacity * ga->data_size);
        if (ga->elements == NULL){
            printf("oopsie doopsie, we couldnt allocate memory for this growin array, the capacity at which the code blew up is: %i \n which is %i bytes",ga->capacity,ga->capacity * ga->data_size);
            exit(1);
        }
    }
    memcpy(
            (char*)ga->elements + index * ga->data_size,
            elem,
            ga->data_size
            );
    ga->size++;
}

void pop(growing_array* ga){
    if (ga->size != 0){
        ga->size--;
        if (ga->size == ga->capacity - INTERVAL){
            ga->capacity -= INTERVAL;
            if (ga->capacity % (INTERVAL * 2) == 0 )
                ga->elements = realloc(ga->elements, ga->capacity * ga->data_size);
            if (ga->elements == NULL){
                printf("oopsie doopsie, we couldnt allocate memory for this growin array, the capacity at which the code blew up is: %i \n which is %i bytes",ga->capacity,ga->capacity * ga->data_size);
                exit(1);
            }
        }
    }
}

void free_array(growing_array *ga){
    free(ga->elements);
}

/*
Example usage of growing_array for bullets in a game:

// Define a bullet struct
typedef struct {
    float x, y;    // position
    float vx, vy;  // velocity
    int damage;    // damage
} Bullet;

// Initialize a growing array for bullets
growing_array bullets = growing_array_init(8, sizeof(Bullet));

// Push a bullet
Bullet b = {100.0f, 200.0f, 0.0f, -5.0f, 10};
push(&bullets, &b);

// Push an anonymous bullet
push(&bullets, &(Bullet){150.0f, 300.0f, 2.0f, -4.0f, 5});

// Access and update bullets
for (int i = 0; i < bullets.size; i++) {
    Bullet* blt = (Bullet*)at(&bullets, i);
    blt->x += blt->vx;
    blt->y += blt->vy;
}

// Remove the last bullet
pop(&bullets);

// Free memory when done
free_array(&bullets);
*/

