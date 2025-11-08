#include "music.h"

Music music;
Sound shootSound;
Sound enemyDieSound;
Sound loseSound;

void InitMusic()
{

    InitAudioDevice();
    music = LoadMusicStream(MUSIC_PATH);
    SetMusicVolume(music, 0.15f);

    shootSound = LoadSound(SHOOT_SOUND_PATH);
    enemyDieSound = LoadSound(ENEMY_DIE_SOUND_PATH);
    loseSound = LoadSound(LOSE_SOUND_PATH);

    SetSoundVolume(shootSound, 0.1f);
    SetSoundVolume(enemyDieSound, 0.2f);
    SetSoundVolume(loseSound, 0.3f);
}

void StartMusic()
{

    SeekMusicStream(music, 0);
    PlayMusicStream(music);
}
