#include <mapRender.h>
#include <gameSound.h>
#include <fstream>
#include <cstdlib>

int blockMap[ROW][COLUMN];
int floorMap[ROW][COLUMN];
int item_width=50;
int index=1;
int startTime=0;
const SDL_Rect smokeRect[2]={{128,0,64,64},{64,0,64,64}};
extern bool soundOn;

void initMap(SDL_Renderer* renderer){
    floor.setRenderer(renderer);
    floor_p1.setRenderer(renderer);
    floor_p2.setRenderer(renderer);
    stone.setRenderer(renderer);
    brick.setRenderer(renderer);
    giftBox.setRenderer(renderer);
    gameTime.setRenderer(renderer);
    gameTime.loadFont("asset/font/zorque.ttf",30);
    timeBorder.setRenderer(renderer);
    timeBorder.loadImage("asset/menu/time_border.png");
    timeBorder.setSize(125,42);
    gameLand.setRenderer(renderer);
    gameLand.loadFont("asset/font/zorque.ttf",25);
    landBorder.setRenderer(renderer);
    landBorder.loadImage("asset/menu/land_border.png");
    exploSmoke.setRenderer(renderer);
    exploSmoke.setSize(64,64);
    exploSmoke.loadImage("asset/bomb/bomb_effect/explo_smoke.png");
    lockOff.setRenderer(renderer);
    lockOff.setSize(64,64);
    lockOff.loadImage("asset/item/lock_off.png");
    lockOn.setRenderer(renderer);
    lockOn.setSize(64,64);
    lockOn.loadImage("asset/item/lock_on.png");
    doorOpen.setRenderer(renderer);
    doorOpen.setSize(64,89);
    doorOpen.loadImage("asset/item/dooropen.png");
    doorClose.setRenderer(renderer);
    doorClose.setSize(64,64);
    doorClose.loadImage("asset/item/doorclosed.png");
    i_bomb_incre.setRenderer(renderer);
    i_bomb_incre.setSize(50,50);
    i_bomb_incre.loadImage("asset/item/bomb_incre.png");
    i_bomb_powerup.setRenderer(renderer);
    i_bomb_powerup.setSize(50,50);
    i_bomb_powerup.loadImage("asset/item/bomb_power.png");
    i_speedup.setRenderer(renderer);
    i_speedup.setSize(50,50);
    i_speedup.loadImage("asset/item/speedup.png");
    i_shield.setRenderer(renderer);
    i_shield.setSize(50,50);
    i_shield.loadImage("asset/item/shield.png");
    i_skull.setRenderer(renderer);
    i_skull.setSize(50,50);
    i_skull.loadImage("asset/item/skull.png");

}

void renderFloor(bool lock_on,SDL_Point lockBlock){
    SDL_Rect floor_rect={0,0,blockWidth*2,blockWidth*2};
    for(int i=0;i<ROW/2+1;i++){
        floor_rect.y=i*blockWidth*2;
        for(int j=0;j<COLUMN/2+1;j++){
            floor_rect.x=j*blockWidth*2;
            floor.render(0,0,&floor_rect);
        }
    }
    if(lockBlock.x!=0 && lockBlock.y!=0){
        if(lock_on){
            lockOn.render(lockBlock.x*blockWidth,lockBlock.y*blockWidth);
            doorOpen.render((COLUMN-2)*blockWidth,(ROW-2)*blockWidth);
        }
        else{
            if(blockMap[lockBlock.y][lockBlock.x]>=START_ITEM_REGION)
                blockMap[lockBlock.y][lockBlock.x]=1;
            lockOff.render(lockBlock.x*blockWidth,lockBlock.y*blockWidth);
            doorClose.render((COLUMN-2)*blockWidth,(ROW-2)*blockWidth);
        }
    }
}

void renderFloor_landMode(){
    SDL_Rect floor_rect={0,0,blockWidth,blockWidth};
    for(int i=0;i<ROW;i++){
        floor_rect.y=i*blockWidth;
        for(int j=0;j<COLUMN;j++){
            floor_rect.x=j*blockWidth;
            if(floorMap[i][j]==FLOOR_DEF)
                floor.render(0,0,&floor_rect);
            else if(floorMap[i][j]==FLOOR_P1)
                floor_p1.render(0,0,&floor_rect);
            else if(floorMap[i][j]==FLOOR_P2)
                floor_p2.render(0,0,&floor_rect);
        }
    }
}

void resetFloor(){
    for(int i=0;i<ROW;i++)
        for(int j=0;j<COLUMN;j++)
            floorMap[i][j]=0;
}

void loadMap(int MAP){
    std::ifstream mapFile;
    switch(MAP){
        case S_TOWN:    mapFile.open("asset/map_files/singleplayer/town.txt");
                        floor.loadImage("asset/map_asset/town/floor.png");
                        stone.loadImage("asset/map_asset/town/stone.png");
                        brick.loadImage("asset/map_asset/town/brick.png");
                        giftBox.loadImage("asset/map_asset/town/gift_box.png");
                        if(soundOn && !Mix_PlayingMusic())
                            Mix_PlayMusic(town_music,-1);
                        break;
        case S_DESERT:  mapFile.open("asset/map_files/singleplayer/desert.txt");
                        floor.loadImage("asset/map_asset/desert/floor.png");
                        stone.loadImage("asset/map_asset/desert/stone.png");
                        brick.loadImage("asset/map_asset/desert/brick.png");
                        giftBox.loadImage("asset/map_asset/desert/gift_box.png");
                        if(soundOn && !Mix_PlayingMusic())
                            Mix_PlayMusic(desert_music,-1);
                        break;
        case S_XMAS:    mapFile.open("asset/map_files/singleplayer/xmas.txt");
                        floor.loadImage("asset/map_asset/xmas/floor.png");
                        stone.loadImage("asset/map_asset/xmas/stone.png");
                        brick.loadImage("asset/map_asset/xmas/brick.png");
                        giftBox.loadImage("asset/map_asset/xmas/gift_box.png");
                        if(soundOn && !Mix_PlayingMusic())
                            Mix_PlayMusic(xmas_music,-1);
                        break;
        case S_UNDERWATER:  mapFile.open("asset/map_files/singleplayer/underwater.txt");
                            floor.loadImage("asset/map_asset/underwater/floor.png");
                            stone.loadImage("asset/map_asset/underwater/stone.png");
                            brick.loadImage("asset/map_asset/underwater/brick.png");
                            giftBox.loadImage("asset/map_asset/underwater/gift_box.png");
                            if(soundOn && !Mix_PlayingMusic())
                                Mix_PlayMusic(underwater_music,-1);
                            break;
        case M_TOWN:    mapFile.open("asset/map_files/multiplayer/town.txt");
                        floor.loadImage("asset/map_asset/town/floor.png");
                        stone.loadImage("asset/map_asset/town/stone.png");
                        brick.loadImage("asset/map_asset/town/brick.png");
                        giftBox.loadImage("asset/map_asset/town/gift_box.png");
                        if(soundOn && !Mix_PlayingMusic())
                            Mix_PlayMusic(town_music,-1);
                        break;
        case M_DESERT:  mapFile.open("asset/map_files/multiplayer/desert.txt");
                        floor.loadImage("asset/map_asset/desert/floor.png");
                        stone.loadImage("asset/map_asset/desert/stone.png");
                        brick.loadImage("asset/map_asset/desert/brick.png");
                        giftBox.loadImage("asset/map_asset/desert/gift_box.png");
                        if(soundOn && !Mix_PlayingMusic())
                            Mix_PlayMusic(desert_music,-1);
                        break;
        case M_XMAS:    mapFile.open("asset/map_files/multiplayer/xmas.txt");
                        floor.loadImage("asset/map_asset/xmas/floor.png");
                        stone.loadImage("asset/map_asset/xmas/stone.png");
                        brick.loadImage("asset/map_asset/xmas/brick.png");
                        giftBox.loadImage("asset/map_asset/xmas/gift_box.png");
                        if(soundOn && !Mix_PlayingMusic())
                            Mix_PlayMusic(xmas_music,-1);
                        break;
        case M_UNDERWATER:  mapFile.open("asset/map_files/multiplayer/underwater.txt");
                            floor.loadImage("asset/map_asset/underwater/floor.png");
                            stone.loadImage("asset/map_asset/underwater/stone.png");
                            brick.loadImage("asset/map_asset/underwater/brick.png");
                            giftBox.loadImage("asset/map_asset/underwater/gift_box.png");
                            if(soundOn && !Mix_PlayingMusic())
                                Mix_PlayMusic(underwater_music,-1);
                            break;
        case L_MAP1:    mapFile.open("asset/map_files/multiplayer/landmap1.txt");
                        floor.loadImage("asset/map_asset/landmode/floor.png");
                        floor_p1.loadImage("asset/map_asset/landmode/floor_p1.png");
                        floor_p2.loadImage("asset/map_asset/landmode/floor_p2.png");
                        stone.loadImage("asset/map_asset/landmode/stone.png");
                        brick.loadImage("asset/map_asset/landmode/brick.png");
                        giftBox.loadImage("asset/map_asset/landmode/gift_box.png");
                        if(soundOn && !Mix_PlayingMusic())
                            Mix_PlayMusic(town_music,-1);
                        break;
        case L_MAP2:    mapFile.open("asset/map_files/multiplayer/landmap2.txt");
                        floor.loadImage("asset/map_asset/landmode/floor.png");
                        floor_p1.loadImage("asset/map_asset/landmode/floor_p1.png");
                        floor_p2.loadImage("asset/map_asset/landmode/floor_p2.png");
                        stone.loadImage("asset/map_asset/landmode/stone.png");
                        brick.loadImage("asset/map_asset/landmode/brick.png");
                        giftBox.loadImage("asset/map_asset/landmode/gift_box.png");
                        if(soundOn && !Mix_PlayingMusic())
                            Mix_PlayMusic(town_music,-1);
                        break;
    }
    for(int i=0;i<ROW;i++)
        for(int j=0;j<COLUMN;j++)
            mapFile>>blockMap[i][j];
    mapFile.close();
}

void renderItem(gameRender& Item,const SDL_Rect block_rect){
    Item.setSize(item_width,item_width);
    Item.render(block_rect.x+(64-item_width)/2,block_rect.y+20+(64-item_width)/2);
}

void renderBlock(){
    SDL_Rect block_rect={0,0,65,89};
    for(int i=0;i<ROW;i++){
            block_rect.y=i*64-25;
        for(int j=0;j<COLUMN;j++){
            block_rect.x=j*64;
            if(blockMap[i][j]==STONE)
                stone.render(0,0,&block_rect);
            else if(blockMap[i][j]==BRICK)
                brick.render(0,0,&block_rect);
            else if(blockMap[i][j]==GIFT_BOX)
                giftBox.render(0,0,&block_rect);
            else if(blockMap[i][j]<=SMOKE){
                SDL_Rect srcRect;
                if(blockMap[i][j]<-90)
                    srcRect=smokeRect[0];
                else
                    srcRect=smokeRect[1];
                exploSmoke.render(block_rect.x,block_rect.y+20,nullptr,&srcRect);
                blockMap[i][j]++;
                if(blockMap[i][j]==SMOKE+1)
                    blockMap[i][j]=E_BLOCK;
            }
            else if(blockMap[i][j]==I_BOMB_INCRE)
                renderItem(i_bomb_incre,block_rect);
            else if(blockMap[i][j]==I_BOMB_POWERUP)
                renderItem(i_bomb_powerup,block_rect);
            else if(blockMap[i][j]==I_SPEED_UP)
                renderItem(i_speedup,block_rect);
            else if(blockMap[i][j]==I_SHIELD)
                renderItem(i_shield,block_rect);
            else if(blockMap[i][j]==I_SKULL)
                renderItem(i_skull,block_rect);
        }
    }
    //item animation
    item_width-=index;
    if(item_width<=35 || item_width>=50)
        index=-index;
}

void resetTime(){
    startTime=SDL_GetTicks();
}

bool renderTime(){
    Uint32 timeRemain=90-(SDL_GetTicks()-startTime)/1000;
    Uint32 minute=timeRemain/60;
    Uint32 second=timeRemain%60;
    std::string s;
    if(second>=10)
        s="0"+gameTime.intToString(minute)+" : "+gameTime.intToString(second);
    else
        s="0"+gameTime.intToString(minute)+" : "+"0"+gameTime.intToString(second);
    if(timeRemain!=0)
        gameTime.renderString(WHITE,s);
    else
        gameTime.renderString(RED,s);
    SDL_Rect timeRect={0,0,0,0};
    gameTime.setTextureSize(timeRect.w,timeRect.h);
    timeRect.x=SCREEN_WIDTH/2-timeRect.w/2;
    timeBorder.render(417,-2);
    gameTime.render(0,0,&timeRect);
    if(timeRemain==0)
        return true;
    return false;
};

bool checkSinglePWIN(SDL_Point playerBlock,SDL_Point lockBlock,bool& lock_on){
    if(playerBlock.x==lockBlock.x && playerBlock.y==lockBlock.y && !lock_on){
        if(soundOn)
            Mix_PlayChannel(1,door_sound,0);
        lock_on=true;
    }
    else if(playerBlock.x==COLUMN-2 && playerBlock.y==ROW-2 && lock_on)
        return true;
    return false;
}

void pauseTime(int timeBeforePausing){
    startTime+=SDL_GetTicks()-timeBeforePausing;
}

bool renderLandOcupied(){
    int p1_land_count=0,p1_land_percent=0,p2_land_count=0,p2_land_percent=0;
    for(int i=0;i<ROW;i++)
        for(int j=0;j<COLUMN;j++)
            if(floorMap[i][j]==FLOOR_P1)
                p1_land_count++;
            else if(floorMap[i][j]==FLOOR_P2)
                p2_land_count++;
    p1_land_percent=p1_land_count*100/(ROW*COLUMN-72);
    p2_land_percent=p2_land_count*100/(ROW*COLUMN-72);
    std::string p1,p2;
    if(p1_land_percent>=10)
        p1=gameLand.intToString(p1_land_percent)+"%";
    else
        p1="0"+gameLand.intToString(p1_land_percent)+"%";
    if(p2_land_percent>=10)
        p2=gameLand.intToString(p2_land_percent)+"%";
    else
        p2="0"+gameLand.intToString(p2_land_percent)+"%";
    SDL_Rect percent_rect={250,0,0,0},landBorderRect={220,-8,106,44};
    gameLand.renderString(GREEN,p1);
    gameLand.setTextureSize(percent_rect.w,percent_rect.h);
    landBorder.render(0,0,&landBorderRect);
    percent_rect.x=landBorderRect.x+landBorderRect.w/2-percent_rect.w/2;
    gameLand.render(0,0,&percent_rect);
    gameLand.renderString(RED,p2);
    gameLand.setTextureSize(percent_rect.w,percent_rect.h);
    landBorderRect.x+=400;
    percent_rect.x=landBorderRect.x+landBorderRect.w/2-percent_rect.w/2;
    landBorder.render(0,0,&landBorderRect);
    gameLand.render(0,0,&percent_rect);
    return  p1_land_count<p2_land_count;
}
