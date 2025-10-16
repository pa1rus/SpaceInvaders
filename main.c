#include "raylib.h"
#include "stdlib.h"

#define SAVE_DATA_FILE "save.data"
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

static void InitGame();
static void UpdateGame(Music music);
static void UpdateInput();
static void InitShoot();
static void UpdateShoot();
static void InitEnemy();
static void UpdateEnemy();
static void IncreaseDifficulty();
static void DrawGame();
static void UpdateDrawFrame(Music music);
static bool SaveValue(unsigned int position, int value);
static int LoadValue(unsigned int position);

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);

    InitGame();

    InitAudioDevice();
    music = LoadMusicStream("SpaceInvaders.mp3");
    SetMusicVolume(music, 0.15f);
    PlayMusicStream(music);
    

    while(!WindowShouldClose()){

        UpdateDrawFrame(music);

    }

    UnloadMusicStream(music);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}

void InitGame(){

    gameOver = false;
    pause = false;
    score = 0;
    highScore = LoadValue(0);
    currentlevel = 0;

    player.rec.width = 30;
    player.rec.height = 30;
    player.rec.x =  SCREEN_WIDTH / 2 - player.rec.width / 2;
    player.rec.y = SCREEN_HEIGHT - 50 - player.rec.height;
    player.speed.x = 500;
    player.speed.y = 500;
    player.color = WHITE;

    minEnemySpawncooldown = 250;
    maxEnemySpawncooldown = 500;
    nextEnemySpawnCooldown = 0;
    lastEnemySpawnTime = 0;

    for (int i = 0; i < MAX_SHOOTS; i++)
    {
        shoot[i].rec.width = 5;
        shoot[i].rec.height = 20;
        shoot[i].speed.x = 0;
        shoot[i].speed.y = 700;
        shoot[i].active = false;
        shoot[i].color = RED;
    }

    for (int j = 0; j < MAX_ENEMIES; j++){

        enemy[j].rec.width = 20;
        enemy[j].rec.height = 20;
        enemy[j].speed.x = 0;
        enemy[j].speed.y = 250;
        enemy[j].active = false;
        enemy[j].color = PURPLE;
    }
}

void UpdateGame(Music music){

    UpdateMusicStream(music);

    if (!gameOver){

        if (!pause){

            currentTime = GetTime();

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

    DrawText("HIGHSCORE", SCREEN_WIDTH / 2 - MeasureText("HIGHSCORE", 30) / 2, SCREEN_HEIGHT / 4 - 160, 30, DARKGRAY);
    DrawText(TextFormat("%d", highScore), SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", highScore), 50) / 2, SCREEN_HEIGHT / 4 - 120, 50, GRAY);

    DrawText("SCORE", SCREEN_WIDTH / 2 - MeasureText("SCORE", 30) / 2, SCREEN_HEIGHT / 4 - 40, 30, DARKGRAY);
    DrawText(TextFormat("%d", score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", score), 50) / 2, SCREEN_HEIGHT / 4, 50, GRAY);

    DrawRectangleRec(player.rec, player.color);

    for (int j = 0; j < MAX_ENEMIES; j++){

        if (enemy[j].active) DrawRectangleRec(enemy[j].rec, enemy[j].color);
    }

    for (int i = 0; i < MAX_SHOOTS; i++)
    {
        if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
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

void UpdateDrawFrame(Music music)
{
    UpdateGame(music);
    DrawGame();
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
