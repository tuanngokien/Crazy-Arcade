#ifndef MAPRENDER_H
#define MAPRENDER_H

#include <gameRender.h>
extern int blockMap[ROW][COLUMN];
extern int floorMap[ROW][COLUMN];
enum {S_TOWN=1,S_XMAS,S_DESERT,S_UNDERWATER,M_TOWN,M_XMAS,M_DESERT,M_UNDERWATER,L_MAP1,L_MAP2};
static gameRender timeBorder;
static gameRender gameTime;
static gameRender landBorder;
static gameRender gameLand;
static gameRender floor;
static gameRender floor_p1;
static gameRender floor_p2;
static gameRender stone;
static gameRender brick;
static gameRender giftBox;
static gameRender lockOff;
static gameRender lockOn;
static gameRender doorOpen;
static gameRender doorClose;
static gameRender exploSmoke;

static gameRender i_bomb_incre;
static gameRender i_bomb_powerup;
static gameRender i_speedup;
static gameRender i_shield;
static gameRender i_skull;

const int UP=-1;
const int DOWN=1;
const int LEFT=-2;
const int RIGHT=2;

const int SMOKE_FRAME_DEFAULT=-150;
const int SMOKE=-1;
const int FLOOR_DEF=0;
const int FLOOR_P1=1;
const int FLOOR_P2=2;
const int STONE=0;
const int E_BLOCK=1;
const int BRICK=2;
const int GIFT_BOX=3;
const int BOMB_SET=4;
const int EXPLOSIONING=5;
const int BOT1_POS=6;
const int BOT2_POS=7;
const int BOT3_POS=8;
const int BOT4_POS=9;
const int TOTAL_ITEM=5;
const int START_ITEM_REGION=10; // if >= ITEM REGION--- this block is current an item, 10 is bomb_incre,....
const int I_BOMB_INCRE=10;
const int I_BOMB_POWERUP=11;
const int I_SPEED_UP=12;
const int I_SHIELD=13;
const int I_SKULL=14;

void initMap(SDL_Renderer* renderer);
void renderFloor(bool lock_on =false,SDL_Point lockBlock ={0,0});
void renderFloor_landMode();
void resetFloor();
void loadMap(int MAP);
void renderBlock();
void renderItem(gameRender& Item,const SDL_Rect block_rect);
void renderBomb();
bool renderTime();
void resetTime();
bool checkSinglePWIN(SDL_Point playerBlock,SDL_Point lockBlock,bool& lock_on);
void pauseTime(int timeBeforePausing);
bool renderLandOcupied();
#endif // MAPRENDER_H
