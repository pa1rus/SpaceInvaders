#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "bullets.h"

typedef struct
{
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player;

extern Player player;

void InitPlayer();
void UpdateInput();

#endif
