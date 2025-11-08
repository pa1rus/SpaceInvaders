#include "bullets.h"

Array bullets;

float shootCooldown = 0.15;
float lastShootTime = 0;

void InitShoots()
{

    arrayInit(&bullets, 8, sizeof(Bullet));
}

void InitShoot()
{

    PlaySound(shootSound);

    Bullet b = {
        .rec = {
            player.rec.x + player.rec.width / 2 - 3,
            player.rec.y - 24,
            6,
            24},
        .speed = {0, 1000},
        .color = RED};

    arrayPush(&bullets, &b);
}

void UpdateShoots()
{

    for (int i = 0; i < bullets.size; i++)
    {

        Bullet *b = arrayAt(&bullets, i);

        b->rec.y -= b->speed.y * GetFrameTime();

        for (int j = 0; j < enemies.size; j++)
        {

            Enemy *e = arrayAt(&enemies, j);

            if (CheckCollisionRecs(b->rec, e->rec))
            {

                score += 100;

                PlaySound(enemyDieSound);
                SpawnParticles((Vector2){e->rec.x + e->rec.width / 2, e->rec.y + e->rec.height / 2}, enemyParticleConfig);

                arraySwapRemove(&bullets, i);
                i--;

                arraySwapRemove(&enemies, j);
                j--;
            }
        }

        if (b->rec.y < -b->rec.height)
        {

            arraySwapRemove(&bullets, i);
            i--;
        }
    }
}
