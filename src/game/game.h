#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "globals.h"
#include "enemies.h"
#include "saves.h"
#include "animations.h"
#include "music.h"

extern bool gameOver;
extern bool pause;
extern int score;
extern int highScore;
extern int currentlevel;
extern float currentTime;

void InitGame();
void UpdateGame();
void DrawGame();
void UpdateDrawFrame();
void IncreaseDifficulty();
void Lose();
void UnloadGame();

#endif
