#ifndef CHARACTER_H
#define CHARACTER_H

#include <gameRender.h>
#include <mapRender.h>
#include <bomb.h>
#include <vector>


const int charWidth=50;
const int charHeight=61;

const SDL_Rect playerUP={0,224,charWidth,charHeight};
const SDL_Rect playerDOWN={0,0,charWidth,charHeight};
const SDL_Rect playerLEFT={0,76,charWidth,charHeight};
const SDL_Rect playerRIGHT={0,151,charWidth,charHeight};

const SDL_Rect effectFrame[5]={{0,0,200,100},{200,0,200,100},{400,0,200,100},{600,0,200,100}};

const SDL_Point TOP_LEFT={73,41};
const SDL_Point BOT_LEFT={73,553};
const SDL_Point TOP_RIGHT={841,41};
const SDL_Point BOT_RIGHT={841,553};
const int boundUP=39;
const int boundDOWN=37;
const int boundLEFT=35;
const int boundRIGHT=33;
const int maxVel=10;
const int defaultVel=2;

static gameRender* boz;
static gameRender* evie;
static gameRender* ike;
static gameRender* plunk;
static gameRender* grave;
static gameRender p1_arrow;
static gameRender p2_arrow;

static gameRender shieldEffect;
static gameRender skullEffect;

const int totalSkullEff=4;
enum skull{none=0,revStep,uncontrolBomb,fastSpeed,slowSpeed};

class character{
private:
    gameRender *Character;
    int CHARACTER;
    int playerID;
    int posX;
    int posY;
    int directState;
    bool running;
    bool dead;
    int velX;
    int velY;
    int currentVel;
    int bombUsed;
    int bombTotal;
    std::vector <SDL_Point> Bomb;
    int bombLength;
    bool shield;
    int shieldCountdown;
    bool skullObsess;
    int skullCountdown;
    skull SkullEff;
    int currentVelTemp;
    int reviveCountdown;

public:
    character(int CHARACTER,int playerID,SDL_Point initPos);
    static void initCharacter(SDL_Renderer* renderer);
    void renderOpeningScene(int playerID);
    int getX() const;
    int getY() const;
    int getDirectState() const;
    void setPosition(SDL_Point);
    void setVelocityX(int velX);
    void setVelocityY(int velY);
    void renderPlayer();
    bool checkCollision(int directState);
    void setBomb();
    void bombCountdown();
    void checkDead();
    bool getDeadState() const;
    void setPlayerDead();
    int getCurrentVel() const;
    void bombIncre();
    SDL_Point getBlock();
    void shieldCountDown();
    void skullCountDown();
    void checkItemCatch();
    void reverseStep();
    void uncontrolSpeedSet(int speed);
    void reviveCharacter();
    void setSkullEffect();
    bool getShieldState();
    bool getSkullState();
    static void freeCharacter();
};

#endif //CHARACTER_H
