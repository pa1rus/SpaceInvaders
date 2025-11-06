#include "enemies.h"

Enemy enemy[MAX_ENEMIES] = { 0 };

int minEnemySpawncooldown = 0;
int maxEnemySpawncooldown = 0;
float nextEnemySpawnCooldown = 0;
float lastEnemySpawnTime = 0;

void InitEnemies(){

    for (int j = 0; j < MAX_ENEMIES; j++){

        enemy[j].rec.width = 24;
        enemy[j].rec.height = 24;
        enemy[j].speed.x = 0;
        enemy[j].speed.y = 250;
        enemy[j].active = false;
        enemy[j].color = PURPLE;

        minEnemySpawncooldown = 250;
        maxEnemySpawncooldown = 500;
        nextEnemySpawnCooldown = 0;
        lastEnemySpawnTime = 0;
    }
}

void SpawnEnemy(){

    for (int i = 0; i < MAX_ENEMIES; i++){

        if (enemy[i].active) continue;

        enemy[i].active = true;
        enemy[i].rec.x = GetRandomValue(0, SCREEN_WIDTH - enemy[i].rec.width);
        enemy[i].rec.y = 0 - enemy[i].rec.height;
        break;
    }
}

void UpdateEnemy(){

    if (score / 1000 >= currentlevel){

        currentlevel++;
        IncreaseDifficulty();
    }

    if (currentTime - lastEnemySpawnTime >= nextEnemySpawnCooldown) {

        nextEnemySpawnCooldown = (float) GetRandomValue(minEnemySpawncooldown, maxEnemySpawncooldown) / 1000;
        lastEnemySpawnTime = currentTime;
        SpawnEnemy();
    }

    for (int i = 0; i < MAX_ENEMIES; i++){

        if (!enemy[i].active) continue;

        enemy[i].rec.y += enemy[i].speed.y * GetFrameTime();

        if (CheckCollisionRecs(enemy[i].rec, player.rec)) {

            enemy[i].active = false;
            InitParticles((Vector2) {enemy[i].rec.x + enemy[i].rec.width / 2, enemy[i].rec.y + enemy[i].rec.height / 2}, enemyParticleNumber);
            Lose();
            break;
        }

        if (enemy[i].rec.y >= SCREEN_HEIGHT) {

            enemy[i].active = false;
        }
    }
}
