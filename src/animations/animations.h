#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "raylib.h"
#include "game.h"

typedef struct {
    Rectangle frameRec;
    int frames;
    int frameCounter;
    int currentFrame;
    int frameSpeed;
    bool repeat;
    bool isFinished;
    float frameWidth;
} Animation;

extern Animation enemyAnimation;
extern Animation playerAnimation;
extern Animation playerDieAnimation;

extern Texture2D playerSprite;
extern Texture2D playerAnimationSprite;
extern Texture2D playerDieAnimationSprite;
extern Texture2D enemySprite;
extern Texture2D shootSprite;
extern Texture2D background0;
extern Texture2D background1;

extern float scrolling0;
extern float scrolling0Speed;
extern float scrolling1;
extern float scrolling1Speed;

void LoadTextures();
void InitAnimations();
void UpdateAnimations();
void UpdateBackground();

#endif
