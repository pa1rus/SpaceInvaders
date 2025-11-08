#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "music.h"
#include "animations.h"

#if defined(PLATFORM_WEB)
    #include "emscripten/emscripten.h"
#endif

int main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");

    InitMusic();
    LoadTextures();

    InitGame();

    #if defined(PLATFORM_WEB)

        emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
        
    #else

        SetTargetFPS(60);

        while (!WindowShouldClose()) 
        {

            UpdateDrawFrame();
        }
    #endif

    UnloadGame();
    CloseWindow();
    return 0;
}
