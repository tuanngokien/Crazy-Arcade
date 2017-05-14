#include <gameSound.h>

Mix_Music* menu_music=nullptr;
Mix_Chunk* button_press=nullptr;
Mix_Chunk* confirm_sound=nullptr;
Mix_Chunk* start_game=nullptr;
Mix_Music* xmas_music=nullptr;
Mix_Music* town_music=nullptr;
Mix_Music* desert_music=nullptr;
Mix_Music* underwater_music=nullptr;
Mix_Chunk* run_sound=nullptr;
Mix_Chunk* door_sound=nullptr;
Mix_Chunk* explo_sound=nullptr;
Mix_Chunk* shield_sound=nullptr;
Mix_Chunk* explosion_sound=nullptr;
Mix_Chunk* charDead_sound=nullptr;
Mix_Chunk* botDead_sound=nullptr;
Mix_Chunk* dead_sound=nullptr;
Mix_Chunk* setBomb_sound=nullptr;
Mix_Chunk* skull_sound=nullptr;
Mix_Chunk* levelFail_sound=nullptr;
Mix_Chunk* win_sound=nullptr;
Mix_Chunk* revive_sound=nullptr;

void initSound(){
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    Mix_VolumeMusic(70);
    menu_music=Mix_LoadMUS("asset/sound/menu.wav");
    xmas_music=Mix_LoadMUS("asset/sound/xmas.wav");
    town_music=Mix_LoadMUS("asset/sound/town.wav");
    desert_music=Mix_LoadMUS("asset/sound/desert.wav");
    underwater_music=Mix_LoadMUS("asset/sound/underwater.wav");
    run_sound=Mix_LoadWAV("asset/sound/run.ogg");
    Mix_VolumeChunk(run_sound,20);
    shield_sound=Mix_LoadWAV("asset/sound/shield.ogg");
    Mix_VolumeChunk(shield_sound,50);
    door_sound=Mix_LoadWAV("asset/sound/door_open.ogg");
    explo_sound=Mix_LoadWAV("asset/sound/explosion.ogg");
    Mix_VolumeChunk(explo_sound,50);
    charDead_sound=Mix_LoadWAV("asset/sound/char_dead.ogg");
    Mix_VolumeChunk(charDead_sound,50);
    botDead_sound=Mix_LoadWAV("asset/sound/bot_dead.ogg");
    Mix_VolumeChunk(botDead_sound,100);
    setBomb_sound=Mix_LoadWAV("asset/sound/setbomb.ogg");
    skull_sound=Mix_LoadWAV("asset/sound/skull.ogg");
    Mix_VolumeChunk(skull_sound,50);
    levelFail_sound=Mix_LoadWAV("asset/sound/levelfail.ogg");
    Mix_VolumeChunk(levelFail_sound,70);
    win_sound=Mix_LoadWAV("asset/sound/levelcomplete.ogg");
    Mix_VolumeChunk(win_sound,70);
    button_press=Mix_LoadWAV("asset/sound/button_press.wav");
    Mix_VolumeChunk(button_press,70);
    confirm_sound=Mix_LoadWAV("asset/sound/confirm.wav");
    Mix_VolumeChunk(button_press,70);
    start_game=Mix_LoadWAV("asset/sound/start.wav");
    revive_sound=Mix_LoadWAV("asset/sound/revive.wav");
}

void destroySound(){

    Mix_FreeMusic(menu_music);
    Mix_FreeChunk(button_press);
    Mix_FreeChunk(confirm_sound);
    Mix_FreeMusic(xmas_music);
    Mix_FreeMusic(town_music);
    Mix_FreeMusic(desert_music);
    Mix_FreeMusic(underwater_music);
    Mix_FreeChunk(run_sound);
    Mix_FreeChunk(door_sound);
    Mix_FreeChunk(explo_sound);
    Mix_FreeChunk(shield_sound);
    Mix_FreeChunk(explosion_sound);
    Mix_FreeChunk(setBomb_sound);
    Mix_FreeChunk(charDead_sound);
    Mix_FreeChunk(botDead_sound);
    Mix_FreeChunk(skull_sound);
    Mix_FreeChunk(levelFail_sound);
    Mix_FreeChunk(win_sound);
    Mix_FreeChunk(start_game);
    Mix_FreeChunk(revive_sound);
}
