#ifndef PARTICLES_H
#define PARTICLES_H

#include "raylib.h"
#include "globals.h"

typedef struct {
    Rectangle rec;
    Vector2 position;
    Vector2 speed;
    Color color;
    float alpha;
    float alphaSpeed;
    float rotation;
    bool active;
} Particle;

extern Particle enemyParticles[];

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
