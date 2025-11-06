#include "bullets.h"

Shoot shoot[MAX_SHOOTS] = { 0 };

float shootCooldown = 0.15;
float lastShootTime = 0;

void InitShoots(){

    for (int i = 0; i < MAX_SHOOTS; i++)
    {
        shoot[i].rec.width = 6;
        shoot[i].rec.height = 24;
        shoot[i].speed.x = 0;
        shoot[i].speed.y = 1000;
        shoot[i].active = false;
        shoot[i].color = RED;
    }

}

void InitShoot(){

    PlaySound(shootSound);

    for (int i = 0; i < MAX_SHOOTS; i++){

        if(shoot[i].active) continue;

        shoot[i].rec.x = player.rec.x + player.rec.width / 2 - shoot[i].rec.width / 2;
        shoot[i].rec.y = player.rec.y - shoot[i].rec.height;
        shoot[i].active = true;

        break;
    }
}

void UpdateShoots(){

    for (int i = 0; i < MAX_SHOOTS; i++){

        if (!shoot[i].active) continue;

        shoot[i].rec.y -= shoot[i].speed.y * GetFrameTime();

        for (int j = 0; j < MAX_ENEMIES; j++){

            if (!enemy[j].active) continue;

            if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec)) {

                score += 100;
                shoot[i].active = false;
                enemy[j].active = false;
                PlaySound(enemyDieSound);
                SpawnParticles((Vector2) {enemy[j].rec.x + enemy[j].rec.width / 2, enemy[j].rec.y + enemy[j].rec.height / 2}, enemyParticleNumber);
            }
        }

        if (shoot[i].rec.y < 0 - shoot[i].rec.height) shoot[i].active = false;
    }
}
