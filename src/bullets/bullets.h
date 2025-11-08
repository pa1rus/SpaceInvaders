#ifndef BULLETS_H
#define BULLETS_H

#include "raylib.h"
#include "globals.h"
#include "player.h"
#include "music.h"
#include "enemies.h"
#include "array.h"
#include "particles.h"

typedef struct
{
    Rectangle rec;
    Vector2 speed;
    Color color;
} Bullet;

extern Array bullets;

extern float shootCooldown;
extern float lastShootTime;

void InitShoots();
void InitShoot();
void UpdateShoots();

#endif
