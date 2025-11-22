#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "music.h"
#include "animations.h"

#if defined(PLATFORM_WEB)
    #include "emscripten/emscripten.h"
#endif

RenderTexture2D target;
int scaledW, scaledH;
int offsetX = 0;
int offsetY = 0;

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");

    target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    InitMusic();
    LoadTextures();

    InitGame();

    #if defined(PLATFORM_WEB)

        emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
        
    #else

        SetTargetFPS(60);

        while (!WindowShouldClose()) 
        {
            int screenW = GetScreenWidth();
            int screenH = GetScreenHeight();

            if(screenW >= screenH){

                scaledH = screenH;
                scaledW = screenH;
                offsetX = (screenW - scaledW) / 2;
                offsetY = 0;
            }
            else {

                scaledW = screenW;
                scaledH = screenW;
                offsetX = 0;
                offsetY = (screenH - scaledH) / 2;
            }
            
            UpdateDrawFrame();
        }
    #endif

    UnloadGame();
    CloseWindow();
    return 0;
}
