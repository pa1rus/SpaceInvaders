#ifndef PARTICLES_H
#define PARTICLES_H

#include "raylib.h"
#include "globals.h"
#include "array.h"

typedef struct
{
    Rectangle rec;
    Vector2 position;
    Vector2 speed;
    Color color;
    float alpha;
    float alphaSpeed;
    float rotation;
} Particle;

typedef struct
{
    int minR, maxR;
    int minG, maxG;
    int minB, maxB;
    int minFadeOutSpeed, maxFadeOutSpeed;
    int minSpeed, maxSpeed;
    int minSize, maxSize;
    int burst;
} ParticleConfig;

extern Array enemyParticles;
extern ParticleConfig enemyParticleConfig;

extern int minEnemyParticleFadeOutSpeed;
extern int maxEnemyParticleFadeOutSpeed;
extern int minEnemyParticleSpeed;
extern int maxEnemyParticleSpeed;
extern int minEnemyParticleSize;
extern int maxEnemyParticleSize;
extern int enemyParticleNumber;

void InitParticles();
void SpawnParticles();
void UpdateParticles();

#endif
