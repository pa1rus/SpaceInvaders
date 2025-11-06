#ifndef MUSIC_H
#define MUSIC_H

#include "raylib.h"
#include "globals.h"

extern Music music;
extern Sound shootSound;
extern Sound enemyDieSound;
extern Sound loseSound;

void InitMusic();
void StartMusic();

#endif
