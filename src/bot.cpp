#include <bot.h>
#include <gameSound.h>
#include <cstdlib>

extern int frameRate;
extern int blockMap[ROW][COLUMN];
extern bool soundOn;

void bot::initBot(SDL_Renderer* renderer){
    lodumani.setRenderer(renderer);
    lodumani.loadImage("asset/character/lodumani.png");
    ghost.setRenderer(renderer);
    ghost.loadImage("asset/character/ghost.png");
    ghost.setBlendTexture(150);
}

bot::bot(int row,int col){
    dead=false;
    this->posX=col*64+9;
    this->posY=row*64-23;
    velX=0;
    velY=0;
    directState=DOWN;
}

bool bot::getDeadState() const{
    return dead;
}

void bot::checkDead(){
        SDL_Point currBlock=bot::getBlock();
        if(blockMap[currBlock.y][currBlock.x]==EXPLOSIONING){
            if(soundOn)
                Mix_PlayChannel(1,botDead_sound,0);
            dead=true;
        }
}

bool bot::checkCollision(int directState){
    SDL_Point footPos={posX+charWidth/2,posY+charHeight};
    int currRow=footPos.y/blockWidth;
    int currCol=footPos.x/blockWidth;
    bool possible=true;
    switch(directState){
        case UP:    if(!dead && (currRow<2 || (blockMap[currRow-1][currCol]!=E_BLOCK && blockMap[currRow-1][currCol]!=EXPLOSIONING && blockMap[currRow-1][currCol]>=SMOKE+1 && blockMap[currRow-1][currCol]<START_ITEM_REGION) ) && (footPos.y<currRow*blockWidth+boundUP))
                        possible=false;
                    else if(dead && (currRow<2 || blockMap[currRow-1][currCol]==STONE || blockMap[currRow-1][currCol]==BOMB_SET ) && (footPos.y<currRow*blockWidth+boundUP))
                        possible=false;
                    else
                        posX=currCol*blockWidth+9; // align margin
                    break;
        case DOWN:  if(!dead && (currRow>ROW-3 ||( blockMap[currRow+1][currCol]!=E_BLOCK && blockMap[currRow+1][currCol]!=EXPLOSIONING && blockMap[currRow+1][currCol]>=SMOKE+1 && blockMap[currRow+1][currCol]<START_ITEM_REGION)) && (footPos.y>currRow*blockWidth+boundDOWN))
                        possible=false;
                    else if(dead && (currRow>ROW-3 || blockMap[currRow+1][currCol]==STONE || blockMap[currRow+1][currCol]==BOMB_SET  ) && (footPos.y>currRow*blockWidth+boundDOWN))
                        possible=false;
                    else
                        posX=currCol*blockWidth+9;
                    break;
        case LEFT:  if(!dead && (currCol<2 || (blockMap[currRow][currCol-1]!=E_BLOCK && blockMap[currRow][currCol-1]!=EXPLOSIONING && blockMap[currRow][currCol-1]>=SMOKE+1 && blockMap[currRow][currCol-1]<START_ITEM_REGION)) && (footPos.x<currCol*blockWidth+boundLEFT))
                        possible=false;
                    else if(dead && (currCol<2 || blockMap[currRow][currCol-1]==STONE || blockMap[currRow][currCol-1]==BOMB_SET ) && (footPos.x<currCol*blockWidth+boundLEFT))
                        possible=false;
                    else
                        posY=(currRow-1)*blockWidth+41;
                    break;
        case RIGHT: if(!dead && (currCol>COLUMN-3 || (blockMap[currRow][currCol+1]!=E_BLOCK && blockMap[currRow][currCol+1]!=EXPLOSIONING &&  blockMap[currRow][currCol+1]>=SMOKE+1 && blockMap[currRow][currCol+1]<START_ITEM_REGION)) && (footPos.x>currCol*blockWidth+boundRIGHT))
                        possible=false;
                    else if(dead && (currCol>COLUMN-3 || blockMap[currRow][currCol+1]==STONE || blockMap[currRow][currCol+1]==BOMB_SET ) && (footPos.x>currCol*blockWidth+boundRIGHT))
                        possible=false;
                    else
                        posY=(currRow-1)*blockWidth+41;
                    break;
    }
    return possible;
}

void bot::renderBot(character& Character,bool& lock_on,SDL_Point lock_block){
    if(frameRate==0){
        int DirectState[4];
        int numberStateAvailable=0;
        for(int state=LEFT;state<=RIGHT;state++){
            if(state==0)
                state=DOWN;
            if(checkCollision(state) && state!=-directState){
                DirectState[numberStateAvailable]=state;
                numberStateAvailable++;
            }
        }
        if(numberStateAvailable>0)
            directState=DirectState[rand()%numberStateAvailable];
        else
            directState=-directState;
    }
    switch(directState){
        case UP:    velX=0;
                    velY=-botVel;
                    break;
        case DOWN:  velX=0;
                    velY=botVel;
                    break;
        case LEFT:  velX=-botVel;
                    velY=0;
                    break;
        case RIGHT: velX=botVel;
                    velY=0;
                    break;
    }
    if(checkCollision(directState)){
        posX+=velX;
        posY+=velY;
    }
    SDL_Rect botPos={posX,posY,charWidth,charHeight},botIMG;
    switch(directState){
        case UP:    botIMG=playerUP;
                    break;
        case DOWN:  botIMG=playerDOWN;
                    break;
        case LEFT:  botIMG=playerLEFT;
                    break;
        case RIGHT: botIMG=playerRIGHT;
                    break;

    }
    if(!dead){
        botIMG.x+=60*(frameRate/5);
        lodumani.render(0,0,&botPos,&botIMG);
    }
    else
        ghost.render(0,0,&botPos);
    // check player impact the bot
    SDL_Point charBlock=Character.getBlock();
    SDL_Point botBlock=bot::getBlock();
    if(charBlock.x==botBlock.x && charBlock.y == botBlock.y && !dead){
        if(blockMap[charBlock.y][charBlock.x]>=START_ITEM_REGION)
            blockMap[charBlock.y][charBlock.x]==E_BLOCK;
        Character.setPlayerDead();
    }
    else if(charBlock.x==botBlock.x && charBlock.y == botBlock.y && !Character.getShieldState() && !Character.getSkullState())
        Character.setSkullEffect();
    if(botBlock.x==lock_block.x && botBlock.y==lock_block.y && lock_on && !dead){
        if(soundOn)
            Mix_PlayChannel(1,door_sound,0);
        lock_on=false;
    }
}

SDL_Point bot::getBlock(){
    SDL_Point footPos={posX+charWidth/2,posY+charHeight};
    int currRow=footPos.y/blockWidth;
    int currCol=footPos.x/blockWidth;
    return {currCol,currRow};
}

void bot::setPosition(int row,int col){
    this->posX=col*64+9;
    this->posY=row*64-23;
}
