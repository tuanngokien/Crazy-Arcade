#include <character.h>
#include <gameSound.h>
#include <cstdlib>

extern int frameRate;
extern bool soundOn;
bomb* bombMap[ROW][COLUMN];


void character::initCharacter(SDL_Renderer* renderer){
    boz=new gameRender;
    boz->setRenderer(renderer);
    boz->loadImage("asset/character/boz.png");
    evie=new gameRender;
    evie->setRenderer(renderer);
    evie->loadImage("asset/character/evie.png");
    ike=new gameRender;
    ike->setRenderer(renderer);
    ike->loadImage("asset/character/ike.png");
    plunk=new gameRender;
    plunk->setRenderer(renderer);
    plunk->loadImage("asset/character/plunk.png");
    grave=new gameRender;
    grave->setRenderer(renderer);
    grave->loadImage("asset/character/grave.png");
    p1_arrow.setRenderer(renderer);
    p1_arrow.loadImage("asset/character/player1_arrow.png");
    p1_arrow.setSize(40,40);
    p2_arrow.setRenderer(renderer);
    p2_arrow.loadImage("asset/character/player2_arrow.png");
    p2_arrow.setSize(40,40);
    shieldEffect.setRenderer(renderer);
    shieldEffect.loadImage("asset/effect/shield.png");
    shieldEffect.setBlendTexture(200);
    skullEffect.setRenderer(renderer);
    skullEffect.loadImage("asset/effect/skull.png");
    skullEffect.setBlendTexture(200);
}

character::character(int CHARACTER,int playerID, SDL_Point initPos){
    this->CHARACTER=CHARACTER;
    this->playerID=playerID;
    switch(CHARACTER){
        case BOZ: Character=boz;
                  break;
        case EVIE:  Character=evie;
                    break;
        case IKE:   Character=ike;
                    break;
        case PLUNK: Character=plunk;
                    break;
    }
    posX=initPos.x;
    posY=initPos.y;
    velX=0;
    velY=0;
    currentVel=defaultVel;
    running=false;
    dead=false;
    directState=DOWN;
    bombUsed=0;
    bombTotal=1;
    bombLength=1;
    shield=false;
    shieldCountdown=0;
    skullObsess=false;
    skullCountdown=0;
    SkullEff=none;
    currentVelTemp=15;
    reviveCountdown=0;
}

int character::getX() const{
    return posX;
}

int character::getY() const{
    return posY ;
}

void character::renderOpeningScene(int playerID){
    SDL_Rect playerPos={posX,posY,charWidth,charHeight},playerIMG;
    if(frameRate%24<=5)
        playerIMG=playerDOWN;
    else if(frameRate%24<=11)
        playerIMG=playerRIGHT;
    else if(frameRate%24<=17)
        playerIMG=playerUP;
    else
        playerIMG=playerLEFT;
    Character->setBlendTexture(frameRate*10);
    Character->render(0,0,&playerPos,&playerIMG);
    if(playerID==1)
        p1_arrow.render(playerPos.x+8,playerPos.y-55+frameRate%20);
    else if(playerID==2)
        p2_arrow.render(playerPos.x+8,playerPos.y-55+frameRate%20);
}
void character::renderPlayer(){
    running=(velX!=0 || velY!=0)? true:false;
    if(character::checkCollision(directState) && !dead){
        posX+=velX;
        posY+=velY;
    }
    //render the player
    Character->setBlendTexture(255);
    SDL_Rect playerPos={posX,posY,charWidth,charHeight},playerIMG;
    switch(directState){
        case UP:    playerIMG=playerUP;
                    break;
        case DOWN:  playerIMG=playerDOWN;
                    break;
        case LEFT:  playerIMG=playerLEFT;
                    break;
        case RIGHT: playerIMG=playerRIGHT;
                    break;

    }
    if(!dead){
        if(running){
            if(soundOn && !Mix_Playing(0))
                Mix_PlayChannel(0,run_sound,0);
            playerIMG.x+=60*(frameRate/5);
            Character->render(0,0,&playerPos,&playerIMG);
        }
        else{
            playerIMG.x+=120;
            Character->render(0,0,&playerPos,&playerIMG);
        }
    }
    else{
        SDL_Point currBlock=character::getBlock();
        SDL_Rect playerGrave={currBlock.x*blockWidth,currBlock.y*blockWidth-30,blockWidth,blockWidth};
        Character->render(0,0,&playerGrave);
    }
    //if bomb explosing then render it, or update which bombs had blasted then delete it
    SDL_Point bombBlock;
    for(int i=0;i<Bomb.size();i++){
            bombBlock=Bomb[i];
            if(bombMap[bombBlock.y][bombBlock.x]->getBombCountdown()==20) // countdown=20 ---- bomb start explosing
                bombUsed--;
            if(!bombMap[bombBlock.y][bombBlock.x]->checkBombAlive()){ //countdown==-1- --- bomb explosed over
                Bomb.erase(Bomb.begin()+i);
                delete bombMap[bombBlock.y][bombBlock.x];
            }
            else
                bombMap[bombBlock.y][bombBlock.x]->bombBlast();
    }
    //check if player caught an item
    character::checkItemCatch();
    if(shield==true)
        character::shieldCountDown();
    else if(skullObsess==true)
        character::skullCountDown();
}

void character::setVelocityX(int velX){
    if(velX>0)
        directState=RIGHT;
    else if(velX<0)
        directState=LEFT;
    this->velX=velX;
    if(velX!=0)
     this->velY=0;
}

void character::setVelocityY(int velY){
        if(velY>0)
            directState=DOWN;
        else if(velY<0)
            directState=UP;
        this->velY=velY;
        if(velY!=0)
            this->velX=0;
}

bool character::checkCollision(int directState){
    SDL_Point footPos={posX+charWidth/2,posY+charHeight};
    int currRow=footPos.y/blockWidth;
    int currCol=footPos.x/blockWidth;
    bool possible=true;
    switch(directState){
        case UP:    if((currRow<2 || (blockMap[currRow-1][currCol]!=E_BLOCK && blockMap[currRow-1][currCol]!=EXPLOSIONING && blockMap[currRow-1][currCol]>=SMOKE+1 && blockMap[currRow-1][currCol]<START_ITEM_REGION)) && (footPos.y<currRow*blockWidth+boundUP) ){
                        possible=false;
                        posY=(currRow*blockWidth+boundUP)-charHeight-1;// align up border
                    }
                    else
                        posX=currCol*blockWidth+9; // align character to the center when go up
                    break;
        case DOWN:  if((currRow>ROW-3 ||( blockMap[currRow+1][currCol]!=E_BLOCK && blockMap[currRow+1][currCol]!=EXPLOSIONING && blockMap[currRow+1][currCol]>=SMOKE+1 && blockMap[currRow+1][currCol]<START_ITEM_REGION)) && (footPos.y>currRow*blockWidth+boundDOWN) ){
                        possible=false;
                        posY=(currRow*blockWidth+boundDOWN)-charHeight+1;
                    }
                    else
                        posX=currCol*blockWidth+9;
                    break;
        case LEFT:  if((currCol<2 || (blockMap[currRow][currCol-1]!=E_BLOCK && blockMap[currRow][currCol-1]!=EXPLOSIONING && blockMap[currRow][currCol-1]>=SMOKE+1 && blockMap[currRow][currCol-1]<START_ITEM_REGION))&& (footPos.x<currCol*blockWidth+boundLEFT) ){
                        possible=false;
                        posX=(currCol*blockWidth+boundLEFT)-charWidth/2-1;
                    }
                    else
                        posY=(currRow-1)*blockWidth+41;
                    break;
        case RIGHT: if((currCol>COLUMN-3 || (blockMap[currRow][currCol+1]!=E_BLOCK && blockMap[currRow][currCol+1]!=EXPLOSIONING &&  blockMap[currRow][currCol+1]>=SMOKE+1 && blockMap[currRow][currCol+1]<START_ITEM_REGION)) && (footPos.x>currCol*blockWidth+boundRIGHT)){
                        possible=false;
                        posX=(currCol*blockWidth+boundRIGHT)-charWidth/2+1;
                    }
                    else
                        posY=(currRow-1)*blockWidth+41;
                    break;
    }
    return possible;
}

void character::bombCountdown(){
    SDL_Point bombBlock;
    for(int i=0;i<Bomb.size();i++){
        bombBlock=Bomb[i];
        if(bombMap[bombBlock.y][bombBlock.x]->checkBombAlive())
            bombMap[bombBlock.y][bombBlock.x]->bombCountdown();
    }
}

void character::checkDead(){
        SDL_Point footPos={posX+charWidth/2,posY+charHeight};
        if(blockMap[footPos.y/blockWidth][footPos.x/blockWidth]==EXPLOSIONING && !shield){  //bomb has recently blasted here and shield isn't protecting
            if(soundOn)
                Mix_PlayChannel(2,charDead_sound,0);
            Character=grave;
            skullObsess=false;
            reviveCountdown=SDL_GetTicks();
            dead=true;
        }
}

void character::setPlayerDead(){
    if(dead==false && !shield){
        if(soundOn)
                Mix_PlayChannel(2,charDead_sound,0);
        Character=grave;
        skullObsess=false;
        reviveCountdown=SDL_GetTicks();
        SDL_Point currPos=character::getBlock();
        if(blockMap[currPos.y][currPos.x]==BOMB_SET)
            blockMap[currPos.y][currPos.x]==E_BLOCK;
        dead=true;
    }
}

bool character::getDeadState() const{
    return dead;
}


void character::setBomb(){
    SDL_Point footPos={posX+charWidth/2,posY+charHeight};
    int currRow=footPos.y/blockWidth;
    int currCol=footPos.x/blockWidth;
    if((blockMap[currRow][currCol]==E_BLOCK || blockMap[currRow][currCol]<=SMOKE) && bombUsed<bombTotal){
        bombUsed++;
        Bomb.push_back({currCol,currRow});
        bombMap[currRow][currCol]=new bomb(CHARACTER,playerID,bombLength);
        bombMap[currRow][currCol]->setBomb(currCol*blockWidth,currRow*blockWidth);
    }
}

int character::getCurrentVel() const{
    return currentVel;
}

int character::getDirectState() const{
    if(SkullEff!=revStep)
        return directState;
    else
        return -directState;
}

SDL_Point character::getBlock(){
    SDL_Point footPos={posX+charWidth/2,posY+charHeight};
    int currRow=footPos.y/blockWidth;
    int currCol=footPos.x/blockWidth;
    return {currCol,currRow};
}

void character::checkItemCatch(){
    SDL_Point footPos={posX+charWidth/2,posY+charHeight};
    SDL_Point currBlock=character::getBlock();
    if(blockMap[currBlock.y][currBlock.x]>=START_ITEM_REGION)
        // check if player pass the item coordinate
        if((footPos.y>=currBlock.y*blockWidth+25 && footPos.y<=currBlock.y*blockWidth+45) ||( footPos.x<=currBlock.x*blockWidth+25 && footPos.x>=currBlock.x*blockWidth+45)){
            if(blockMap[currBlock.y][currBlock.x]==I_BOMB_INCRE)
                bombTotal++;
            else if(blockMap[currBlock.y][currBlock.x]==I_BOMB_POWERUP)
                bombLength++;
            else if(blockMap[currBlock.y][currBlock.x]==I_SPEED_UP){
                if(SkullEff==slowSpeed || SkullEff==fastSpeed)
                    currentVelTemp++;
                else{
                    if(abs(currentVel)<maxVel){
                        if(currentVel>0)
                            currentVel++;
                        else // skull is obsessing
                            currentVel--;
                        if(velX>0)
                            velX++;
                        else if(velX<0)
                            velX--;
                        else if(velY>0)
                            velY++;
                        else if(velY<0)
                            velY--;
                    }
                }
            }
            else if(blockMap[currBlock.y][currBlock.x]==I_SHIELD && !shield){
                if(skullObsess==true){
                    skullCountdown=0;
                    character::skullCountDown();
                }
                if(soundOn)
                    Mix_PlayChannel(1,shield_sound,0);
                shield=true;
                shieldCountdown=300;
            }
            else if(blockMap[currBlock.y][currBlock.x]==I_SKULL)
                setSkullEffect();
            blockMap[currBlock.y][currBlock.x]=E_BLOCK;
        }
}

void character::shieldCountDown(){
    if(shieldCountdown>0){
        shieldCountdown--;
        SDL_Rect shieldRect={posX,posY+33,charWidth,charWidth/2},frameRect=effectFrame[frameRate/6];
        shieldEffect.render(0,0,&shieldRect,&frameRect);
    }
    else
        shield=false;
}

void character::skullCountDown(){
    if(skullCountdown>0){
        if(SkullEff==revStep && skullCountdown==400)
            character::reverseStep();
        else if(SkullEff==fastSpeed && skullCountdown==400)
            character::uncontrolSpeedSet(20);
        else if(SkullEff==slowSpeed && skullCountdown==400)
            character::uncontrolSpeedSet(1);
        else if(SkullEff==uncontrolBomb)
            character::setBomb();
        skullCountdown--;
        SDL_Rect skullRect={posX,posY+2,charWidth,charWidth/2},frameRect=effectFrame[frameRate/6];
        skullEffect.render(0,0,&skullRect,&frameRect);
    }
    else{
        skullObsess=false;
        if(SkullEff==revStep)
            character::reverseStep();
        else if(SkullEff==fastSpeed || SkullEff==slowSpeed)
            character::uncontrolSpeedSet(currentVelTemp);
        SkullEff=none;
    }
}

void character::reverseStep(){
    currentVel=-currentVel;
    velX=-velX;
    velY=-velY;
    directState=-directState;
}

void character::uncontrolSpeedSet(int speed){
    if(currentVel>=defaultVel && currentVel<=maxVel)
        currentVelTemp=currentVel;
    currentVel=speed;
    if(velX>0)
        velX=currentVel;
    else if(velX<0)
        velX=-currentVel;
    else if(velY>0)
        velY=currentVel;
    else if(velY<0)
        velY=-currentVel;
}

void character::setPosition(SDL_Point pos){
    posX=pos.x;
    posY=pos.y;
}

void character::reviveCharacter(){
    if(SDL_GetTicks()-reviveCountdown>=2000){
        switch(CHARACTER){
            case BOZ: Character=boz;
                      break;
            case EVIE:  Character=evie;
                        break;
            case IKE:   Character=ike;
                        break;
            case PLUNK: Character=plunk;
                        break;
        }
        dead=false;
        velX=0;
        velY=0;
        shield=true;
        shieldCountdown=200;
        if(skullObsess==true){
            skullCountdown=0;
            character::skullCountDown();
        }
        Mix_PlayChannel(0,revive_sound,0);
    }
}

void character::setSkullEffect(){
    if(shield==true)
        shield=false;
    if(skullObsess==true){
        skullCountdown=0;
        character::skullCountDown();
    }
    if(soundOn)
        Mix_PlayChannel(1,skull_sound,0);
    skullObsess=true;
    skullCountdown=400;
    SkullEff=skull(revStep+rand()%totalSkullEff);
}

bool character::getShieldState(){
    return shield;
}

bool character::getSkullState(){
    return skullObsess;
}

void character::freeCharacter(){
    delete boz;
    delete evie;
    delete ike;
    delete plunk;
    delete grave;
}
