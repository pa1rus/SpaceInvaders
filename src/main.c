#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "music.h"
#include "animations.h"

int main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);

    InitMusic();
    LoadTextures();

    InitGame();

    while (!WindowShouldClose())
    {

        UpdateDrawFrame();
    }

    UnloadGame();
    CloseWindow();
    return 0;
}
