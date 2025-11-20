#ifndef SAVES_H
#define SAVES_H

#include "raylib.h"
#include "globals.h"
#include "stdlib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten.h>
#endif

bool SaveValue(int value);
int LoadValue();

#endif
