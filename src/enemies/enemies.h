#ifndef ENEMIES_H
#define ENEMIES_H

#include "raylib.h"
#include "globals.h"
#include "particles.h"
#include "game.h"
#include "player.h"

typedef struct {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

extern Enemy enemy[];

extern int minEnemySpawncooldown;
extern int maxEnemySpawncooldown;
extern float nextEnemySpawnCooldown;
extern float lastEnemySpawnTime;

void InitEnemies();
void SpawnEnemy();
void UpdateEnemy();

#endif
