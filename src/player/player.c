#include "player.h"

Player player = {0};

void InitPlayer()
{

    player.rec.width = 32;
    player.rec.height = 32;
    player.rec.x = SCREEN_WIDTH / 2 - player.rec.width / 2;
    player.rec.y = SCREEN_HEIGHT - 50 - player.rec.height;
    player.speed.x = 500;
    player.speed.y = 500;
    player.color = WHITE;
}

void UpdateInput()
{

    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)))
        player.rec.x -= player.speed.x * GetFrameTime();
    if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)))
        player.rec.x += player.speed.x * GetFrameTime();

    if (player.rec.x <= 0)
        player.rec.x = 0;
    if (player.rec.x + player.rec.width >= SCREEN_WIDTH)
        player.rec.x = SCREEN_WIDTH - player.rec.width;

    if (IsKeyDown(KEY_SPACE) && currentTime - lastShootTime >= shootCooldown)
    {

        InitShoot();
        lastShootTime = currentTime;
    }

    if (IsKeyPressed(KEY_P))
        pause = true;
}
