#ifndef BOT_H
#define BOT_H

#include <gameRender.h>
#include <character.h>

static gameRender lodumani;
static gameRender ghost;
const int botVel=3;

class bot{
private:
    int posX;
    int posY;
    int velX;
    int velY;
    int directState;
    bool dead;
public:
    bot(int row=0, int col=0);
    static void initBot(SDL_Renderer* renderer);
    void setPosition(int row,int col);
    bool getDeadState() const;
    void checkDead();
    bool checkCollision(int directState);
    void renderBot(character& Character,bool& lock_on,SDL_Point lock_block);
    SDL_Point getBlock();
    void reviveBot();
};

#endif // BOT_H
