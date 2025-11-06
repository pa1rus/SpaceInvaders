#include "animations.h"

Animation enemyAnimation = { 0 };
Animation playerAnimation = { 0 };
Animation playerDieAnimation = { 0 };

Texture2D playerSprite;
Texture2D playerAnimationSprite;
Texture2D playerDieAnimationSprite;
Texture2D enemySprite;
Texture2D shootSprite;
Texture2D background0;
Texture2D background1;

float scrolling0 = 0.0f;
float scrolling0Speed = 20;
float scrolling1 = 0.0f;
float scrolling1Speed = 70;

void LoadTextures(){

    playerSprite = LoadTexture(PLAYER_TEXTURE_PATH);
    playerAnimationSprite = LoadTexture(PLAYER_ANIMATION_TEXTURE_PATH);
    playerDieAnimationSprite = LoadTexture(PLAYER_DIE_ANIMATION_TEXTURE_PATH);
    enemySprite = LoadTexture(ENEMY_TEXTURE_PATH);
    shootSprite = LoadTexture(SHOOT_TEXTURE_PATH);
    background0 = LoadTexture(BACKGROUND0_TEXTURE_PATH);
    background1 = LoadTexture(BACKGROUND1_TEXTURE_PATH);
}

void InitAnimations(){

    enemyAnimation.currentFrame = 0;
    enemyAnimation.frameCounter = 0;
    enemyAnimation.frameRec = (Rectangle) {0.0f, 0.0f, (float) enemySprite.width / 2.0f, (float) enemySprite.height};
    enemyAnimation.frames = 1;
    enemyAnimation.frameSpeed = 15;
    enemyAnimation.frameWidth = (float) enemySprite.width / 2.0f;
    enemyAnimation.repeat = true;

    playerAnimation.currentFrame = 0;
    playerAnimation.frameCounter = 0;
    playerAnimation.frameRec = (Rectangle) {0.0f, 0.0f, (float) playerAnimationSprite.width / 4.0f, (float) playerAnimationSprite.height};
    playerAnimation.frames = 3;
    playerAnimation.frameSpeed = 6;
    playerAnimation.frameWidth = (float) playerAnimationSprite.width / 4.0f;
    playerAnimation.repeat = true;

    playerDieAnimation.currentFrame = 0;
    playerDieAnimation.frameCounter = 0;
    playerDieAnimation.frameRec = (Rectangle) {0.0f, 0.0f, (float) playerDieAnimationSprite.width / 10.0f, (float) playerDieAnimationSprite.height};
    playerDieAnimation.frames = 9;
    playerDieAnimation.frameSpeed = 2;
    playerDieAnimation.frameWidth = (float) playerDieAnimationSprite.width / 10.0f;
    playerDieAnimation.repeat = false;
    playerDieAnimation.isFinished = false;
}

void UpdateAnimations(){

    enemyAnimation.frameCounter++;

    if (enemyAnimation.frameCounter >= enemyAnimation.frameSpeed) {

        enemyAnimation.frameCounter = 0;
        enemyAnimation.currentFrame = (enemyAnimation.currentFrame + 1) % 2;
        enemyAnimation.frameRec.x = (float) enemyAnimation.currentFrame * enemyAnimation.frameWidth;
    }

    if (gameOver){

        for (int i = 0; i < MAX_ENEMIES; i++){

            if (enemy[i].speed.y > 0) enemy[i].speed.y -= 6;
        }
    }

    if (!gameOver){

        playerAnimation.frameCounter++;

        if (playerAnimation.frameCounter >= playerAnimation.frameSpeed) {

            playerAnimation.frameCounter = 0;
            playerAnimation.currentFrame++;
            if (playerAnimation.currentFrame >= 4) playerAnimation.currentFrame = 0;
            playerAnimation.frameRec.x = (float) playerAnimation.currentFrame * playerAnimation.frameWidth;
        }
    }
    else if (!playerDieAnimation.isFinished){

        playerDieAnimation.frameCounter++;

        if (playerDieAnimation.frameCounter >= playerDieAnimation.frameSpeed) {

            playerDieAnimation.frameCounter = 0;
            playerDieAnimation.currentFrame++;
            playerDieAnimation.frameRec.x = (float) playerDieAnimation.currentFrame * playerDieAnimation.frameWidth;

            if (playerDieAnimation.currentFrame >= 10) {

                playerDieAnimation.isFinished = true;
            }
        }
    }
}

void UpdateBackground(){

    if (!pause){

            scrolling0 += scrolling0Speed * GetFrameTime();
            scrolling1 += scrolling1Speed * GetFrameTime();

            if (scrolling0 >= background0.height) scrolling0 = 0;
            if (scrolling1 >= background1.height) scrolling1 = 0;
    }

    if (gameOver){

        if (scrolling0Speed > 8) scrolling0Speed -= 10 * GetFrameTime();
        
        if (scrolling1Speed > 16) scrolling1Speed -= 20 * GetFrameTime();
    }
}