#ifndef ENEMIES_H
#define ENEMIES_H

#include "raylib.h"
#include "globals.h"
#include "particles.h"
#include "game.h"
#include "player.h"
#include "array.h"

typedef struct
{
    Rectangle rec;
    Vector2 speed;
    Color color;
} Enemy;

extern Array enemies;

extern int minEnemySpawncooldown;
extern int maxEnemySpawncooldown;
extern float nextEnemySpawnCooldown;
extern float lastEnemySpawnTime;
extern int currentBaseEnemySpeed;

void InitEnemies();
void SpawnEnemy();
void UpdateEnemy();

#endif
