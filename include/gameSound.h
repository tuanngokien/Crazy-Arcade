#ifndef GAMESOUND_H
#define GAMESOUND_H

#include <SDL_mixer.h>

extern Mix_Music* menu_music;
extern Mix_Chunk* button_press;
extern Mix_Chunk* confirm_sound;
extern Mix_Music* xmas_music;
extern Mix_Music* town_music;
extern Mix_Music* desert_music;
extern Mix_Music* underwater_music;
extern Mix_Chunk* run_sound;
extern Mix_Chunk* door_sound;
extern Mix_Chunk* explo_sound;
extern Mix_Chunk* shield_sound;
extern Mix_Chunk* explosion_sound;
extern Mix_Chunk* setBomb_sound;
extern Mix_Chunk* charDead_sound;
extern Mix_Chunk* botDead_sound;
extern Mix_Chunk* skull_sound;
extern Mix_Chunk* levelFail_sound;
extern Mix_Chunk* win_sound;
extern Mix_Chunk* start_game;
extern Mix_Chunk* revive_sound;

void initSound();
void destroySound();

#endif // GAMESOUND_H
