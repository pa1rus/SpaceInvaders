#include "enemies.h"

Array enemies;

int minEnemySpawncooldown = 0;
int maxEnemySpawncooldown = 0;
float nextEnemySpawnCooldown = 0;
float lastEnemySpawnTime = 0;
int currentBaseEnemySpeed = 250;

Enemy baseEnemy = {

    .rec.width = 24,
    .rec.height = 24,
    .speed.x = 0,
    .speed.y = 250,
};

void InitEnemies()
{

    arrayInit(&enemies, 16, sizeof(Enemy));

    minEnemySpawncooldown = 250;
    maxEnemySpawncooldown = 500;
    nextEnemySpawnCooldown = 0;
    lastEnemySpawnTime = 0;
    currentBaseEnemySpeed = 250;
}

void SpawnEnemy()
{

    Enemy e = baseEnemy;

    e.rec.x = GetRandomValue(0, SCREEN_WIDTH - e.rec.width);
    e.rec.y = 0 - e.rec.height;
    e.speed.y = currentBaseEnemySpeed;

    arrayPush(&enemies, &e);
}

void UpdateEnemy()
{

    if (score / 1000 >= currentlevel)
    {

        currentlevel++;
        IncreaseDifficulty();
    }

    if (currentTime - lastEnemySpawnTime >= nextEnemySpawnCooldown)
    {

        nextEnemySpawnCooldown = (float)GetRandomValue(minEnemySpawncooldown, maxEnemySpawncooldown) / 1000;
        lastEnemySpawnTime = currentTime;
        SpawnEnemy();
    }

    for (int i = 0; i < enemies.size; i++)
    {

        Enemy *e = arrayAt(&enemies, i);

        e->rec.y += e->speed.y * GetFrameTime();

        if (CheckCollisionRecs(e->rec, player.rec))
        {
            SpawnParticles((Vector2){e->rec.x + e->rec.width / 2, e->rec.y + e->rec.height / 2}, enemyParticleConfig);

            arraySwapRemove(&enemies, i);
            i--;
            
            Lose();
            break;
        }

        if (e->rec.y >= SCREEN_HEIGHT)
        {

            arraySwapRemove(&enemies, i);
            i--;
        }
    }
}
