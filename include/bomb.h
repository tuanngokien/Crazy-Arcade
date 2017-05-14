#ifndef BOMB_H
#define BOMB_H
#include <gameRender.h>
#include <mapRender.h>
#include <vector>

const SDL_Rect explo_fram1={0,0,103,103};
const SDL_Rect explo_fram2={103,0,103,103};
const SDL_Rect explo_fram3={206,0,103,103};
const SDL_Rect explo_fram4={309,0,103,103};
const SDL_Rect explo_fram5={412,0,103,103};
const SDL_Rect explo_fram6={515,0,103,103};
const SDL_Rect explo_fram7={618,0,103,103};
const SDL_Rect explo_fram8={721,0,103,103};
const SDL_Rect explo_fram9={824,0,103,103};
const SDL_Rect explo_fram10={927,0,103,103};

static gameRender* robot_bomb1;
static gameRender* xmas_bomb;
static gameRender* skull_bomb;
static gameRender* rose_bomb;
static gameRender* ball_bomb;
static gameRender bombFire;
static gameRender explosion;

enum {BOZ=1,EVIE,IKE,PLUNK,LODUMANI};

class bomb{
private:
    static SDL_Renderer* renderer_;
    gameRender* bombSet;
    int bombX;
    int bombY;
    int countdown;
    int bombLength;
    bool bombed;
    int playerID;

public:
    bomb(int Character,int playerID,int bombLength=1);
    static void initBomb(SDL_Renderer* renderer);
    SDL_Point getBlock() const;
    int getBombLength() const;
    void setBomb(int posX,int posY);
    void bombCountdown();
    void bombBlast();
    bool checkBombAlive() const;
    void setBlast();
    void explosionRender(int posX,int posY) const;
    int getBombCountdown();
    static void freeBomb();
};

#endif // BOMB_H
