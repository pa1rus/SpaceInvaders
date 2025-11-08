#include "particles.h"

Array enemyParticles;

ParticleConfig enemyParticleConfig = {
    .minR = 220, .maxR = 255, 
    .minG = 10, .maxG = 60, 
    .minB = 220, .maxB = 255, 
    .minFadeOutSpeed = 12, .maxFadeOutSpeed = 20, 
    .minSpeed = -600, .maxSpeed = 600, 
    .minSize = 80, .maxSize = 160, 
    .burst = 16
};

void InitParticles()
{

    arrayInit(&enemyParticles, 64, sizeof(Particle));
}

void SpawnParticles(Vector2 position, ParticleConfig config)
{

    int particleInitiated = 0;

    Particle p = {

        .alpha = 1.0f};

    for (int i = 0; i < config.burst; i++)
    {

        arrayPush(&enemyParticles, &p);

        float size = (float)GetRandomValue(config.minSize, config.maxSize) / 10.0f;
        float speedX = (float)GetRandomValue(config.minSpeed, config.maxSpeed) / 10.0f;
        float speedY = (float)GetRandomValue(config.minSpeed, config.maxSpeed) / 10.0f;
        float alphaSpeed = (float)GetRandomValue(config.minFadeOutSpeed, config.maxFadeOutSpeed) / 10.0f;
        float rotation = GetRandomValue(0, 359);
        Color color = (Color){GetRandomValue(config.minR, config.maxR), GetRandomValue(config.minG, config.maxG), GetRandomValue(config.minB, config.maxB)};
        p.rec.height = size;
        p.rec.width = size;
        p.speed.x = speedX;
        p.speed.y = speedY;
        p.alphaSpeed = alphaSpeed;
        p.rotation = rotation;
        p.color = color;
        p.rec.x = position.x;
        p.rec.y = position.y;

        particleInitiated++;
    }
}

void UpdateParticles()
{

    for (int i = 0; i < enemyParticles.size; i++)
    {

        Particle *p = (Particle *)arrayAt(&enemyParticles, i);

        p->alpha -= p->alphaSpeed * GetFrameTime();
        p->rec.x -= p->speed.x * GetFrameTime();
        p->rec.y -= p->speed.y * GetFrameTime();

        if (p->alpha <= 0)
        {

            arraySwapRemove(&enemyParticles, i);
            i--;
        }
    }
}
