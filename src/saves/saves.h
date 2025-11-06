#ifndef SAVES_H
#define SAVES_H

#include "raylib.h"
#include "globals.h"
#include "stdlib.h"

bool SaveValue(unsigned int position, int value);
int LoadValue(unsigned int position);

#endif
