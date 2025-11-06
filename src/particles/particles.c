#include "particles.h"

Particle enemyParticles[MAX_PARTICLES] = { 0 };

int minEnemyParticleFadeOutSpeed = 12;
int maxEnemyParticleFadeOutSpeed = 20;
int minEnemyParticleSpeed = -600;
int maxEnemyParticleSpeed = 600;
int minEnemyParticleSize = 80;
int maxEnemyParticleSize = 160;
int enemyParticleNumber = 16;

void InitParticles(){

    for (int k = 0; k < MAX_PARTICLES; k++)
    {
        enemyParticles[k].position = (Vector2) { 0, 0 };
        enemyParticles[k].color = (Color) { GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255 };
        enemyParticles[k].alpha = 1.0f;
        enemyParticles[k].rotation = (float) GetRandomValue(0, 360);
        enemyParticles[k].active = false;
    }
}

void SpawnParticles(Vector2 position, int number){

    int particleInitiated = 0;

    for (int i = 0; i < MAX_PARTICLES; i++){

        if (!enemyParticles[i].active){

            float size = (float) GetRandomValue(minEnemyParticleSize, maxEnemyParticleSize) / 10.0f;
            float speedX = (float) GetRandomValue(minEnemyParticleSpeed, maxEnemyParticleSpeed) / 10.0f;
            float speedY = (float) GetRandomValue(minEnemyParticleSpeed, maxEnemyParticleSpeed) / 10.0f;
            float rotation = GetRandomValue(0, 359);
            float alphaSpeed = (float) GetRandomValue(minEnemyParticleFadeOutSpeed, maxEnemyParticleFadeOutSpeed) / 10.0f;
            Color color = (Color) {GetRandomValue(220, 255), GetRandomValue(10, 60), GetRandomValue(220, 255)};
            enemyParticles[i].rec.height = size;
            enemyParticles[i].rec.width = size;
            enemyParticles[i].speed.x = speedX;
            enemyParticles[i].speed.y = speedY;
            enemyParticles[i].alphaSpeed = alphaSpeed;
            enemyParticles[i].rotation = rotation;
            enemyParticles[i].color = color;
            enemyParticles[i].active = true;
            enemyParticles[i].rec.x = position.x;
            enemyParticles[i].rec.y = position.y;

            particleInitiated++;

            if (particleInitiated >= number) break;
        }
    }
}

void UpdateParticles(){

    for (int i = 0; i < MAX_PARTICLES; i++){


        if(enemyParticles[i].active){

            enemyParticles[i].alpha -= enemyParticles[i].alphaSpeed * GetFrameTime();
            enemyParticles[i].rec.x -= enemyParticles[i].speed.x * GetFrameTime();
            enemyParticles[i].rec.y -= enemyParticles[i].speed.y * GetFrameTime();

            if(enemyParticles[i].alpha <= 0) {

                enemyParticles[i].active = false;
                enemyParticles[i].alpha = 1.0f;
            }
        }
    }
}
