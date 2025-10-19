#include "raylib.h"
#include "stdlib.h"

#define SAVE_DATA_FILE "save.data"
#define MUSIC_PATH "./assets/audio/SpaceInvaders.mp3"
#define PLAYER_TEXTURE_PATH "./assets/textures/player.png"
#define PLAYER_ANIMATION_TEXTURE_PATH "./assets/textures/playerAnimation.png"
#define ENEMY_TEXTURE_PATH "./assets/textures/enemy.png"
#define SHOOT_TEXTURE_PATH "./assets/textures/shoot.png"
#define BACKGROUND0_TEXTURE_PATH "./assets/textures/layer0.png"
#define BACKGROUND1_TEXTURE_PATH "./assets/textures/layer1.png"
#define MAX_SHOOTS 20
#define MAX_ENEMIES 40

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

static float currentTime = 0;

static float shootCooldown = 0.15;
static float lastShootTime = 0;

static int minEnemySpawncooldown = 0;
static int maxEnemySpawncooldown = 0;
static float nextEnemySpawnCooldown = 0;
static float lastEnemySpawnTime = 0;

static Music music;
static Texture2D playerSprite;
static Texture2D playerAnimationSprite;
static Texture2D enemySprite;
static Texture2D shootSprite;
static Texture2D background0;
static Texture2D background1;

static Rectangle enemyFrameRec;
static int currentEnemyFrame = 0;
static int enemyFramesCounter = 0;
static int enemyFramesSpeed = 15;

static Rectangle playerAnimationFrameRec;
static int currentPlayerAnimationFrame = 0;
static int playerAnimationFramesCounter = 0;
static int playerAnimationFramesSpeed = 6;

float scrolling0 = 0.0f;
float scrolling1 = 0.0f;

static void InitGame();
static void UpdateGame();
static void UpdateInput();
static void InitShoot();
static void UpdateShoot();
static void InitEnemy();
static void UpdateEnemy();
static void IncreaseDifficulty();
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
    PlayMusicStream(music);

    playerSprite = LoadTexture(PLAYER_TEXTURE_PATH);
    playerAnimationSprite = LoadTexture(PLAYER_ANIMATION_TEXTURE_PATH);
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

    enemyFrameRec = (Rectangle) {0.0f, 0.0f, (float) enemySprite.width / 2, (float) enemySprite.height};
    playerAnimationFrameRec = (Rectangle) {0.0f, 0.0f, (float) playerAnimationSprite.width / 4, (float) playerAnimationSprite.height};

    for (int i = 0; i < MAX_SHOOTS; i++)
    {
        shoot[i].rec.width = 6;
        shoot[i].rec.height = 24;
        shoot[i].speed.x = 0;
        shoot[i].speed.y = 700;
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
}

void UpdateGame(){

    UpdateMusicStream(music);

    if (!gameOver){

        if (!pause){

            currentTime = GetTime();

            scrolling0 += 20 * GetFrameTime();
            scrolling1 += 70 * GetFrameTime();

            if (scrolling0 >= background0.height) scrolling0 = 0;
            if (scrolling1 >= background1.height) scrolling1 = 0;

            UpdateInput();
            UpdateShoot();
            UpdateEnemy();
        }
        else{

            if (IsKeyPressed(KEY_P)) pause = false;
        }
        
    }
    else{

        if (score > highScore) SaveValue(0, score); 
        if (IsKeyPressed(KEY_ENTER)) InitGame();
    }
}

void UpdateInput(){

    playerAnimationFramesCounter++;

    if (playerAnimationFramesCounter >= playerAnimationFramesSpeed) {

        playerAnimationFramesCounter = 0;
        currentPlayerAnimationFrame++;
        if (currentPlayerAnimationFrame >= 4) currentPlayerAnimationFrame = 0;
        float frameWidth = (float) playerAnimationSprite.width / 4.0f;
        playerAnimationFrameRec.x = (float) currentPlayerAnimationFrame * frameWidth;
    }

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

    enemyFramesCounter++;

    if (enemyFramesCounter >= enemyFramesSpeed) {

        enemyFramesCounter = 0;
        currentEnemyFrame = (currentEnemyFrame + 1) % 2;
        float frameWidth = (float) enemySprite.width / 2.0f;
        enemyFrameRec.x = (float) currentEnemyFrame * frameWidth;
    }

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

        if (CheckCollisionRecs(enemy[i].rec, player.rec)) gameOver = true;

        if (enemy[i].rec.y >= SCREEN_HEIGHT) {

            enemy[i].active = false;
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

    Rectangle src = { 0.0f, 0.0f, (float) playerSprite.width, (float) playerSprite.height };
    Rectangle dest = { player.rec.x, player.rec.y, player.rec.width, player.rec.height };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(playerSprite, src, dest, origin, 0.0f, WHITE);

    float frameWidth = (float) playerAnimationSprite.width / 4.0f;
    Rectangle srcPA = { playerAnimationFrameRec.x, playerAnimationFrameRec.y, frameWidth, (float) playerAnimationSprite.height };
    Rectangle destPA = { player.rec.x + player.rec.width / 4, player.rec.y + player.rec.height, player.rec.width / 2, player.rec.height / 2 };
    Vector2 originPA = { 0.0f, 0.0f };
    DrawTexturePro(playerAnimationSprite, srcPA, destPA, originPA, 0.0f, WHITE);

    for (int j = 0; j < MAX_ENEMIES; j++){

        if (enemy[j].active) {

            float frameWidth = (float) enemySprite.width / 2.0f;
            Rectangle srcE = { enemyFrameRec.x, enemyFrameRec.y, frameWidth, (float) enemySprite.height };
            Rectangle destE = { enemy[j].rec.x, enemy[j].rec.y, enemy[j].rec.width, enemy[j].rec.height };
            Vector2 originE = { 0.0f, 0.0f };
            DrawTexturePro(enemySprite, srcE, destE, originE, 0.0f, WHITE);

        }
    }

    for (int i = 0; i < MAX_SHOOTS; i++)
    {
        if (shoot[i].active) {

            Rectangle srcS = { 0.0f, 0.0f, (float) shootSprite.width, (float) shootSprite.height };
            Rectangle destS = { shoot[i].rec.x, shoot[i].rec.y, shoot[i].rec.width, shoot[i].rec.height };
            Vector2 originS = { 0.0f, 0.0f };
            DrawTexturePro(shootSprite, srcS, destS, originS, 0.0f, WHITE);
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
