#include "game.h"

bool gameOver = false;
bool pause = false;
int score = 0;
int highScore = 0;
int currentlevel = 0;
float currentTime = 0;

void InitGame()
{

    gameOver = false;
    pause = false;
    score = 0;
    highScore = LoadValue();
    currentlevel = 0;

    InitPlayer();
    InitEnemies();
    InitShoots();
    InitAnimations();
    InitParticles();
    StartMusic();
}

void UpdateGame()
{

    UpdateMusicStream(music);

    if (!pause && !gameOver)
    {

        currentTime = GetTime();

        UpdateInput();
        UpdateShoots();
        UpdateEnemy();
        UpdateAnimations();
        UpdateBackground();
        UpdateParticles();
    }
    else
    {

        if (IsKeyPressed(KEY_P))
            pause = false;
    }

    if (gameOver)
    {

        UpdateShoots();
        UpdateEnemy();
        UpdateAnimations();
        UpdateBackground();
        UpdateParticles();

        if (IsKeyPressed(KEY_ENTER))
            InitGame();
    }
}

void DrawGame()
{

    BeginTextureMode(target);

    ClearBackground(BLACK);

    DrawTextureEx(background0, (Vector2){0, scrolling0}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background0, (Vector2){0, -background0.height + scrolling0}, 0.0f, 1.0f, WHITE);

    DrawTextureEx(background1, (Vector2){0, scrolling1}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background1, (Vector2){0, -background1.height + scrolling1}, 0.0f, 1.0f, WHITE);

    DrawText("HIGHSCORE", SCREEN_WIDTH / 2 - MeasureText("HIGHSCORE", 30) / 2, SCREEN_HEIGHT / 4 - 160, 30, GRAY);
    DrawText(TextFormat("%d", highScore), SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", highScore), 50) / 2, SCREEN_HEIGHT / 4 - 120, 50, WHITE);

    DrawText("SCORE", SCREEN_WIDTH / 2 - MeasureText("SCORE", 30) / 2, SCREEN_HEIGHT / 4 - 40, 30, GRAY);
    DrawText(TextFormat("%d", score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", score), 50) / 2, SCREEN_HEIGHT / 4, 50, WHITE);

    if (!gameOver)
    {

        Rectangle srcP = {0.0f, 0.0f, (float)playerSprite.width, (float)playerSprite.height};
        Rectangle destP = {player.rec.x, player.rec.y, player.rec.width, player.rec.height};
        DrawTexturePro(playerSprite, srcP, destP, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

        Rectangle srcPA = {playerAnimation.frameRec.x, playerAnimation.frameRec.y, playerAnimation.frameWidth, (float)playerAnimationSprite.height};
        Rectangle destPA = {player.rec.x + player.rec.width / 4.0f, player.rec.y + player.rec.height, player.rec.width / 2.0f, player.rec.height / 2.0f};
        DrawTexturePro(playerAnimationSprite, srcPA, destPA, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }
    else if (!playerDieAnimation.isFinished)
    {

        Rectangle srcPDA = {playerDieAnimation.frameRec.x, playerDieAnimation.frameRec.y, playerDieAnimation.frameWidth, (float)playerDieAnimationSprite.height};
        Rectangle destPDA = {player.rec.x, player.rec.y, player.rec.width, player.rec.height};
        DrawTexturePro(playerDieAnimationSprite, srcPDA, destPDA, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }

    for (int i = 0; i < enemyParticles.size; i++)
    {

        Particle *p = (Particle *)arrayAt(&enemyParticles, i);

        DrawRectanglePro(p->rec, (Vector2){0.0f, 0.0f}, p->rotation, ColorAlpha(p->color, p->alpha));
    }

    for (int j = 0; j < enemies.size; j++)
    {

        Enemy *e = arrayAt(&enemies, j);

        Rectangle srcE = {enemyAnimation.frameRec.x, enemyAnimation.frameRec.y, enemyAnimation.frameWidth, (float)enemySprite.height};
        Rectangle destE = {e->rec.x, e->rec.y, e->rec.width, e->rec.height};
        DrawTexturePro(enemySprite, srcE, destE, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }

    for (int k = 0; k < bullets.size; k++)
    {
        Bullet *b = (Bullet *)arrayAt(&bullets, k);

        Rectangle srcS = {0, 0, shootSprite.width, shootSprite.height};
        Rectangle destS = {b->rec.x, b->rec.y, b->rec.width, b->rec.height};

        DrawTexturePro(shootSprite, srcS, destS, (Vector2){0, 0}, 0, WHITE);
    }

    if (pause)
    {

        DrawText("PAUSE", SCREEN_WIDTH / 2 - MeasureText("PAUSE", 60) / 2, SCREEN_HEIGHT / 2, 60, WHITE);
    }

    if (gameOver)
    {

        DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 60) / 2, SCREEN_HEIGHT / 2, 60, WHITE);
        DrawText("PRESS ENTER TO RESTART", SCREEN_WIDTH / 2 - MeasureText("PRESS ENTER TO RESTART", 20) / 2, SCREEN_HEIGHT / 2 + 80, 20, GRAY);
    }
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexturePro(
        target.texture,
        (Rectangle){0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT},
        (Rectangle){offsetX, offsetY, scaledW, scaledH},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    EndDrawing();
}

void UpdateDrawFrame()
{
    UpdateGame();
    DrawGame();
}

void IncreaseDifficulty()
{

    currentBaseEnemySpeed *= 1.1;

    for (int i = 0; i < enemies.size; i++)
    {

        Enemy *e = arrayAt(&enemies, i);

        e->speed.y = currentBaseEnemySpeed;
    }

    minEnemySpawncooldown *= 0.9;
    maxEnemySpawncooldown *= 0.8;
}

void Lose()
{

    if (score > highScore)
        SaveValue(score);
    PauseMusicStream(music);
    PlaySound(loseSound);
    gameOver = true;
}

void UnloadGame()
{

    arrayFree(&bullets);

    UnloadMusicStream(music);
    UnloadSound(shootSound);
    UnloadSound(enemyDieSound);
    UnloadSound(loseSound);
    UnloadTexture(playerSprite);
    UnloadTexture(playerAnimationSprite);
    UnloadTexture(shootSprite);
    UnloadTexture(enemySprite);
    UnloadTexture(background0);
    UnloadTexture(background1);
    CloseAudioDevice();
}
