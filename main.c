#include "raylib.h"
#include "stdlib.h"

#define SAVE_DATA_FILE "save.data"
#define MUSIC_PATH "./assets/audio/SpaceInvaders.ogg"
#define SHOOT_SOUND_PATH "./assets/audio/shoot.ogg"
#define ENEMY_DIE_SOUND_PATH "./assets/audio/enemyDie.ogg"
#define LOSE_SOUND_PATH "./assets/audio/lose.ogg"
#define PLAYER_TEXTURE_PATH "./assets/textures/player.png"
#define PLAYER_ANIMATION_TEXTURE_PATH "./assets/textures/playerAnimation.png"
#define PLAYER_DIE_ANIMATION_TEXTURE_PATH "./assets/textures/playerDie.png"
#define ENEMY_TEXTURE_PATH "./assets/textures/enemy.png"
#define SHOOT_TEXTURE_PATH "./assets/textures/shoot.png"
#define BACKGROUND0_TEXTURE_PATH "./assets/textures/layer0.png"
#define BACKGROUND1_TEXTURE_PATH "./assets/textures/layer1.png"
#define MAX_SHOOTS 20
#define MAX_ENEMIES 40
#define MAX_PARTICLES 100

typedef struct {
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player;

typedef struct {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

typedef struct {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot;

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

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 800;
static const int FPS = 60;

static bool gameOver = false;
static bool pause = false;
static int score = 0;
static int highScore = 0;
static int currentlevel = 0;

static Player player = { 0 };
static Enemy enemy[MAX_ENEMIES] = { 0 };
static Shoot shoot[MAX_SHOOTS] = { 0 };
static Particle enemyParticles[MAX_PARTICLES] = { 0 };
static Animation enemyAnimation = { 0 };
static Animation playerAnimation = { 0 };
static Animation playerDieAnimation = { 0 };

static float currentTime = 0;

static float shootCooldown = 0.15;
static float lastShootTime = 0;

static int minEnemySpawncooldown = 0;
static int maxEnemySpawncooldown = 0;
static float nextEnemySpawnCooldown = 0;
static float lastEnemySpawnTime = 0;

static Music music;
static Sound shootSound;
static Sound enemyDieSound;
static Sound loseSound;
static Texture2D playerSprite;
static Texture2D playerAnimationSprite;
static Texture2D playerDieAnimationSprite;
static Texture2D enemySprite;
static Texture2D shootSprite;
static Texture2D background0;
static Texture2D background1;

float scrolling0 = 0.0f;
float scrollin0Speed = 20;
float scrolling1 = 0.0f;
float scrollin1Speed = 70;

int minEnemyParticleFadeOutSpeed = 12;
int maxEnemyParticleFadeOutSpeed = 20;
int minEnemyParticleSpeed = -600;
int maxEnemyParticleSpeed = 600;
int minEnemyParticleSize = 80;
int maxEnemyParticleSize = 160;
int enemyParticleNumber = 16;

static void InitGame();
static void UpdateGame();
static void UpdateInput();
static void InitShoot();
static void UpdateShoot();
static void InitEnemy();
static void UpdateEnemy();
static void InitParticles(Vector2 position, int number);
static void UpdateParticles();
static void UpdateBackground();
static void UpdateAnimations();
static void IncreaseDifficulty();
static void Lose();
static void DrawGame();
static void UpdateDrawFrame();
static void UnloadGame();
static bool SaveValue(unsigned int position, int value);
static int LoadValue(unsigned int position);

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);

    InitAudioDevice();
    music = LoadMusicStream(MUSIC_PATH);
    SetMusicVolume(music, 0.15f);

    shootSound = LoadSound(SHOOT_SOUND_PATH);
    enemyDieSound = LoadSound(ENEMY_DIE_SOUND_PATH);
    loseSound = LoadSound(LOSE_SOUND_PATH);

    SetSoundVolume(shootSound, 0.1f);
    SetSoundVolume(enemyDieSound, 0.2f);
    SetSoundVolume(loseSound, 0.3f);

    playerSprite = LoadTexture(PLAYER_TEXTURE_PATH);
    playerAnimationSprite = LoadTexture(PLAYER_ANIMATION_TEXTURE_PATH);
    playerDieAnimationSprite = LoadTexture(PLAYER_DIE_ANIMATION_TEXTURE_PATH);
    enemySprite = LoadTexture(ENEMY_TEXTURE_PATH);
    shootSprite = LoadTexture(SHOOT_TEXTURE_PATH);
    background0 = LoadTexture(BACKGROUND0_TEXTURE_PATH);
    background1 = LoadTexture(BACKGROUND1_TEXTURE_PATH);

    InitGame();

    while(!WindowShouldClose()){

        UpdateDrawFrame();

    }

    UnloadGame();
    CloseWindow();
    return 0;
}

void InitGame(){

    SeekMusicStream(music, 0);
    PlayMusicStream(music);
    gameOver = false;
    pause = false;
    score = 0;
    highScore = LoadValue(0);
    currentlevel = 0;

    player.rec.width = 32;
    player.rec.height = 32;
    player.rec.x =  SCREEN_WIDTH / 2 - player.rec.width / 2;
    player.rec.y = SCREEN_HEIGHT - 50 - player.rec.height;
    player.speed.x = 500;
    player.speed.y = 500;
    player.color = WHITE;

    minEnemySpawncooldown = 250;
    maxEnemySpawncooldown = 500;
    nextEnemySpawnCooldown = 0;
    lastEnemySpawnTime = 0;

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

    for (int i = 0; i < MAX_SHOOTS; i++)
    {
        shoot[i].rec.width = 6;
        shoot[i].rec.height = 24;
        shoot[i].speed.x = 0;
        shoot[i].speed.y = 1000;
        shoot[i].active = false;
        shoot[i].color = RED;
    }

    for (int j = 0; j < MAX_ENEMIES; j++){

        enemy[j].rec.width = 24;
        enemy[j].rec.height = 24;
        enemy[j].speed.x = 0;
        enemy[j].speed.y = 250;
        enemy[j].active = false;
        enemy[j].color = PURPLE;
    }

    for (int k = 0; k < MAX_PARTICLES; k++)
    {
        enemyParticles[k].position = (Vector2) { 0, 0 };
        enemyParticles[k].color = (Color) { GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255 };
        enemyParticles[k].alpha = 1.0f;
        enemyParticles[k].rotation = (float) GetRandomValue(0, 360);
        enemyParticles[k].active = false;
    }
}

void UpdateGame(){

    UpdateMusicStream(music);

    if (!pause && !gameOver){

        currentTime = GetTime();

        UpdateInput();
        UpdateShoot();
        UpdateEnemy(); 
        UpdateAnimations();
        UpdateBackground();
        UpdateParticles();
    }
    else{

        if (IsKeyPressed(KEY_P)) pause = false;
    }

    if (gameOver){

        UpdateShoot();
        UpdateEnemy(); 
        UpdateAnimations();
        UpdateBackground();
        UpdateParticles();

        if (IsKeyPressed(KEY_ENTER)) InitGame();
        
    }
}

void UpdateInput(){

    if((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))) player.rec.x -= player.speed.x * GetFrameTime();
    if((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))) player.rec.x += player.speed.x * GetFrameTime();

    if (player.rec.x <= 0) player.rec.x = 0;
    if (player.rec.x + player.rec.width >= SCREEN_WIDTH) player.rec.x = SCREEN_WIDTH - player.rec.width;

    if(IsKeyDown(KEY_SPACE) && currentTime - lastShootTime >= shootCooldown){

        InitShoot();
        lastShootTime = currentTime;
    }

    if (IsKeyPressed(KEY_P)) pause = true;
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

void UpdateShoot(){

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
                InitParticles((Vector2) {enemy[j].rec.x + enemy[j].rec.width / 2, enemy[j].rec.y + enemy[j].rec.height / 2}, enemyParticleNumber);
            }
        }

        if (shoot[i].rec.y < 0 - shoot[i].rec.height) shoot[i].active = false;
    }
}

void InitEnemy(){

    for (int i = 0; i < MAX_ENEMIES; i++){

        if (enemy[i].active) continue;

        enemy[i].active = true;
        enemy[i].rec.x = GetRandomValue(0, SCREEN_WIDTH - enemy[i].rec.width);
        enemy[i].rec.y = 0 - enemy[i].rec.height;
        break;
    }

    
}

void UpdateEnemy(){

    if (score / 1000 >= currentlevel){

        currentlevel++;
        IncreaseDifficulty();
    }

    if (currentTime - lastEnemySpawnTime >= nextEnemySpawnCooldown) {

        nextEnemySpawnCooldown = (float) GetRandomValue(minEnemySpawncooldown, maxEnemySpawncooldown) / 1000;
        lastEnemySpawnTime = currentTime;
        InitEnemy();
    }

    for (int i = 0; i < MAX_ENEMIES; i++){

        if (!enemy[i].active) continue;

        enemy[i].rec.y += enemy[i].speed.y * GetFrameTime();

        if (CheckCollisionRecs(enemy[i].rec, player.rec)) {

            enemy[i].active = false;
            InitParticles((Vector2) {enemy[i].rec.x + enemy[i].rec.width / 2, enemy[i].rec.y + enemy[i].rec.height / 2}, enemyParticleNumber);
            Lose();
            break;
        }

        if (enemy[i].rec.y >= SCREEN_HEIGHT) {

            enemy[i].active = false;
        }
    }
}

void InitParticles(Vector2 position, int number){

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

void UpdateBackground(){

    if (!pause){

            scrolling0 += scrollin0Speed * GetFrameTime();
            scrolling1 += scrollin1Speed * GetFrameTime();

            if (scrolling0 >= background0.height) scrolling0 = 0;
            if (scrolling1 >= background1.height) scrolling1 = 0;
    }

    if (gameOver){

        if (scrollin0Speed > 8) scrollin0Speed -= 10 * GetFrameTime();
        
        if (scrollin1Speed > 16) scrollin1Speed -= 20 * GetFrameTime();
    }
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

void IncreaseDifficulty(){

    for (int i = 0; i < MAX_ENEMIES; i++){

        enemy[i].speed.y *= 1.1;
    }

    minEnemySpawncooldown *= 0.9;
    maxEnemySpawncooldown *= 0.8;
}

void Lose(){

    if (score > highScore) SaveValue(0, score); 
    PauseMusicStream(music);
    PlaySound(loseSound);
    gameOver = true;
            
}

void DrawGame(){

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTextureEx(background0, (Vector2){ 0, scrolling0 }, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background0, (Vector2){ 0, -background0.height + scrolling0 }, 0.0f, 1.0f, WHITE);

    DrawTextureEx(background1, (Vector2){ 0, scrolling1 }, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background1, (Vector2){ 0, -background1.height + scrolling1 }, 0.0f, 1.0f, WHITE);

    DrawText("HIGHSCORE", SCREEN_WIDTH / 2 - MeasureText("HIGHSCORE", 30) / 2, SCREEN_HEIGHT / 4 - 160, 30, GRAY);
    DrawText(TextFormat("%d", highScore), SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", highScore), 50) / 2, SCREEN_HEIGHT / 4 - 120, 50, WHITE);

    DrawText("SCORE", SCREEN_WIDTH / 2 - MeasureText("SCORE", 30) / 2, SCREEN_HEIGHT / 4 - 40, 30, GRAY);
    DrawText(TextFormat("%d", score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", score), 50) / 2, SCREEN_HEIGHT / 4, 50, WHITE);

    if (!gameOver){

        Rectangle srcP = { 0.0f, 0.0f, (float) playerSprite.width, (float) playerSprite.height };
        Rectangle destP = { player.rec.x, player.rec.y, player.rec.width, player.rec.height };
        DrawTexturePro(playerSprite, srcP, destP, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);

        Rectangle srcPA = { playerAnimation.frameRec.x, playerAnimation.frameRec.y, playerAnimation.frameWidth, (float) playerAnimationSprite.height };
        Rectangle destPA = { player.rec.x + player.rec.width / 4.0f, player.rec.y + player.rec.height, player.rec.width / 2.0f, player.rec.height / 2.0f };
        DrawTexturePro(playerAnimationSprite, srcPA, destPA, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
    }
    else if (!playerDieAnimation.isFinished){

        Rectangle srcPDA = { playerDieAnimation.frameRec.x, playerDieAnimation.frameRec.y, playerDieAnimation.frameWidth, (float) playerDieAnimationSprite.height };
        Rectangle destPDA = { player.rec.x, player.rec.y, player.rec.width, player.rec.height };
        DrawTexturePro(playerDieAnimationSprite, srcPDA, destPDA, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
    }

    for (int i = 0; i < MAX_PARTICLES; i++){

        if (enemyParticles[i].active){

            DrawRectanglePro(enemyParticles[i].rec, (Vector2) { 0.0f, 0.0f }, enemyParticles[i].rotation,  ColorAlpha(enemyParticles[i].color , enemyParticles[i].alpha));
        }
    }

    for (int j = 0; j < MAX_ENEMIES; j++){

        if (enemy[j].active) {

            Rectangle srcE = { enemyAnimation.frameRec.x, enemyAnimation.frameRec.y, enemyAnimation.frameWidth, (float) enemySprite.height };
            Rectangle destE = { enemy[j].rec.x, enemy[j].rec.y, enemy[j].rec.width, enemy[j].rec.height };
            DrawTexturePro(enemySprite, srcE, destE, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);

        }
    }

    for (int k = 0; k < MAX_SHOOTS; k++)
    {
        if (shoot[k].active) {

            Rectangle srcS = { 0.0f, 0.0f, (float) shootSprite.width, (float) shootSprite.height };
            Rectangle destS = { shoot[k].rec.x, shoot[k].rec.y, shoot[k].rec.width, shoot[k].rec.height };
            DrawTexturePro(shootSprite, srcS, destS, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
        }
    }

    if (pause) {

        DrawText("PAUSE", SCREEN_WIDTH / 2 - MeasureText("PAUSE", 60) / 2, SCREEN_HEIGHT / 2, 60, WHITE);
    }

    if (gameOver) {

        DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 60) / 2, SCREEN_HEIGHT / 2, 60, WHITE);
        DrawText("PRESS ENTER TO RESTART", SCREEN_WIDTH / 2 - MeasureText("PRESS ENTER TO RESTART", 20) / 2, SCREEN_HEIGHT / 2 + 80, 20, GRAY);
    }

    EndDrawing();
}

void UpdateDrawFrame()
{
    UpdateGame();
    DrawGame();
}

void UnloadGame(){

    UnloadMusicStream(music);
    UnloadSound(shootSound);
    UnloadSound(enemyDieSound);
    UnloadSound(loseSound);
    UnloadTexture(playerSprite);
    UnloadTexture(playerAnimationSprite);
    UnloadTexture(shootSprite);
    UnloadTexture(enemySprite);
    CloseAudioDevice();
}

bool SaveValue(unsigned int position, int value){

    bool success = false;
    int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(SAVE_DATA_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL){

        if (dataSize <= position * sizeof(int)){

            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                TraceLog(LOG_ERROR, "Error");

                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else{

            newFileData = fileData;
            newDataSize = dataSize;

            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(SAVE_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);
    }
    else{

        dataSize = (position + 1) * sizeof(int);
        fileData = (unsigned char *) RL_MALLOC(dataSize);
        int *dataPtr = (int *) fileData;
        dataPtr[position] = value;

        success = SaveFileData(SAVE_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);
    }

    return success;
}

int LoadValue(unsigned int position){

    int value = 0;
    int dataSize = 0;
    unsigned char *fileData = LoadFileData(SAVE_DATA_FILE, &dataSize);

    if (fileData != NULL){

        if (dataSize < ((int) position * sizeof(int))) TraceLog(LOG_ERROR, "Error");
        else {

            int *dataPtr = (int *) fileData;
            value = dataPtr[position];
        }
    }

    UnloadFileData(fileData);

    return value;
}
