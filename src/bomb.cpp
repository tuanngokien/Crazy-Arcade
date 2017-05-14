#include <bomb.h>
#include <cstdlib>
#include <gameSound.h>

extern int frameRate;
extern bomb* bombMap[ROW][COLUMN];
extern bool soundOn;

SDL_Renderer* bomb::renderer_;
void bomb::initBomb(SDL_Renderer* renderer){
        renderer_=renderer;
        robot_bomb1=new gameRender;
        robot_bomb1->setRenderer(renderer);
        robot_bomb1->loadImage("asset/bomb/robot_bomb.png");
        xmas_bomb=new gameRender;
        xmas_bomb->setRenderer(renderer);
        xmas_bomb->loadImage("asset/bomb/xmas_bomb.png");
        skull_bomb=new gameRender;
        skull_bomb->setRenderer(renderer);
        skull_bomb->loadImage("asset/bomb/skull_bomb.png");
        rose_bomb=new gameRender;
        rose_bomb->setRenderer(renderer);
        rose_bomb->loadImage("asset/bomb/rose_bomb.png");
        ball_bomb=new gameRender;
        ball_bomb->setRenderer(renderer);
        ball_bomb->loadImage("asset/bomb/ball_bomb.png");
        bombFire.setRenderer(renderer);
        bombFire.loadImage("asset/bomb/bomb_effect/bomb_fire.png");
        bombFire.setSize(20,20);
        explosion.setRenderer(renderer);
        explosion.setSize(70,70);
        explosion.loadImage("asset/bomb/bomb_effect/explosion.png");
}

bomb::bomb(int Character,int playerID, int bombLength){
    switch(Character){
        case BOZ:   bombSet=robot_bomb1;
                    break;
        case EVIE:  bombSet=rose_bomb;
                    break;
        case IKE:   bombSet=ball_bomb;
                    break;
        case PLUNK: bombSet=xmas_bomb;
                    break;
        case LODUMANI:  bombSet=skull_bomb;
                        break;
    }
    bombX=0;
    bombY=0;
    countdown=0;
    this->playerID=playerID;
    this->bombLength=bombLength;
    bombed=false;
}

void bomb::setBomb(int posX,int posY){
    if(bombed==false){
        if(soundOn)
            Mix_PlayChannel(0,setBomb_sound,0);
        this->bombX=posX;
        this->bombY=posY;
        this->bombed=true;
        this->countdown=150;
    }
}

void bomb::bombCountdown(){
    if(bombed==true)
         if(countdown>=20){
            blockMap[bombY/blockWidth][bombX/blockWidth]=BOMB_SET;
            bombSet->setSize(blockWidth-8-frameRate/4,blockWidth-8-frameRate/4);
            bombSet->render(bombX+2+frameRate/8,bombY+2+frameRate/8);
            SDL_Rect fireRect={(frameRate/4)*66,0,66,68};
            bombFire.render(bombX+5+frameRate/8,bombY-2+frameRate/8,nullptr,&fireRect);
         }
    countdown--;
}

int bomb::getBombCountdown(){
    return countdown;
}

void bomb::bombBlast(){
     if(bombed==true){
         if(countdown>=0 && countdown <20){
            if(countdown==19){
                if(soundOn)
                    Mix_PlayChannel(2,explo_sound,0);
                SDL_Rect explo_display={5,7,SCREEN_WIDTH,SCREEN_HEIGHT};
                SDL_RenderSetViewport(renderer_,&explo_display);
                blockMap[bombY/blockWidth][bombX/blockWidth]=EXPLOSIONING;
            }
            else if(countdown==15){
                SDL_Rect explo_ori={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
                SDL_RenderSetViewport(renderer_,&explo_ori);
            }
            else if(countdown==12){
                switch(playerID){
                    case 0: break;
                    case 1: floorMap[bombY/blockWidth][bombX/blockWidth]=FLOOR_P1;
                            break;
                    case 2: floorMap[bombY/blockWidth][bombX/blockWidth]=FLOOR_P2;
                            break;
                }
            }
            bomb::explosionRender(bombX,bombY);
            //eplosing down
            for(int i=1;i<=bombLength;i++){
                if(blockMap[bombY/blockWidth+i][bombX/blockWidth]==E_BLOCK || blockMap[bombY/blockWidth+i][bombX/blockWidth]<=SMOKE || blockMap[bombY/blockWidth+i][bombX/blockWidth]==EXPLOSIONING){
                    explosionRender(bombX,bombY+i*blockWidth);
                    if(countdown==19)
                        blockMap[bombY/blockWidth+i][bombX/blockWidth]=EXPLOSIONING;
                    else if(countdown==12){
                        switch(playerID){
                            case 0: break;
                            case 1: floorMap[bombY/blockWidth+i][bombX/blockWidth]=FLOOR_P1;
                                break;
                            case 2: floorMap[bombY/blockWidth+i][bombX/blockWidth]=FLOOR_P2;
                                    break;
                        }
                    }
                    else if(countdown==0)
                        blockMap[bombY/blockWidth+i][bombX/blockWidth]=SMOKE_FRAME_DEFAULT;
                }
                else if(blockMap[bombY/blockWidth+i][bombX/blockWidth]==BRICK || blockMap[bombY/blockWidth+i][bombX/blockWidth]==GIFT_BOX || blockMap[bombY/blockWidth+i][bombX/blockWidth]>=START_ITEM_REGION ){
                    explosionRender(bombX,bombY+i*blockWidth);
                    explosionRender(bombX+30,bombY+i*blockWidth);
                    explosionRender(bombX-30,bombY+i*blockWidth);
                    explosionRender(bombX,bombY+i*blockWidth+30);
                    explosionRender(bombX,bombY+i*blockWidth-30);
                    if(countdown==12){
                        switch(playerID){
                            case 0: break;
                            case 1: floorMap[bombY/blockWidth+i][bombX/blockWidth]=FLOOR_P1;
                                break;
                            case 2: floorMap[bombY/blockWidth+i][bombX/blockWidth]=FLOOR_P2;
                                    break;
                        }
                    }
                    else if(countdown==0){
                        if(blockMap[bombY/blockWidth+i][bombX/blockWidth]==BRICK){
                            blockMap[bombY/blockWidth+i][bombX/blockWidth]=9+rand()%(TOTAL_ITEM+1);
                            if(blockMap[bombY/blockWidth+i][bombX/blockWidth]<START_ITEM_REGION)//=9
                                blockMap[bombY/blockWidth+i][bombX/blockWidth]=SMOKE_FRAME_DEFAULT;
                        }
                        else if(blockMap[bombY/blockWidth+i][bombX/blockWidth]==GIFT_BOX){ //100% contain an item
                            blockMap[bombY/blockWidth+i][bombX/blockWidth]=9+rand()%(TOTAL_ITEM+1);
                            if(blockMap[bombY/blockWidth+i][bombX/blockWidth]<START_ITEM_REGION)
                                blockMap[bombY/blockWidth+i][bombX/blockWidth]=I_BOMB_INCRE;
                        }
                        else if(blockMap[bombY/blockWidth+i][bombX/blockWidth]>=START_ITEM_REGION)
                            blockMap[bombY/blockWidth+i][bombX/blockWidth]=SMOKE_FRAME_DEFAULT;
                    }
                    break;
                }
                else if(blockMap[bombY/blockWidth+i][bombX/blockWidth]==BOMB_SET){
                        bombMap[bombY/blockWidth+i][bombX/blockWidth]->setBlast();
                        break;
                }
                else
                    break;
            }
            //explosing up
            for(int i=1;i<=bombLength;i++){
                if(blockMap[bombY/blockWidth-i][bombX/blockWidth]==E_BLOCK || blockMap[bombY/blockWidth-i][bombX/blockWidth]<=SMOKE || blockMap[bombY/blockWidth-i][bombX/blockWidth]==EXPLOSIONING){
                    explosionRender(bombX,bombY-i*blockWidth);
                    if(countdown==19)
                        blockMap[bombY/blockWidth-i][bombX/blockWidth]=EXPLOSIONING;
                    else if(countdown==12){
                        switch(playerID){
                            case 0: break;
                            case 1: floorMap[bombY/blockWidth-i][bombX/blockWidth]=FLOOR_P1;
                                break;
                            case 2: floorMap[bombY/blockWidth-i][bombX/blockWidth]=FLOOR_P2;
                                    break;
                        }
                    }
                    else if(countdown==0)
                        blockMap[bombY/blockWidth-i][bombX/blockWidth]=SMOKE_FRAME_DEFAULT;
                }
                else if(blockMap[bombY/blockWidth-i][bombX/blockWidth]==BRICK || blockMap[bombY/blockWidth-i][bombX/blockWidth]==GIFT_BOX || blockMap[bombY/blockWidth-i][bombX/blockWidth]>=START_ITEM_REGION){
                    explosionRender(bombX,bombY-i*blockWidth);
                    explosionRender(bombX+30,bombY-i*blockWidth);
                    explosionRender(bombX-30,bombY-i*blockWidth);
                    explosionRender(bombX,bombY-i*blockWidth+30);
                    explosionRender(bombX,bombY-i*blockWidth-30);
                    if(countdown==12){
                        switch(playerID){
                            case 0: break;
                            case 1: floorMap[bombY/blockWidth-i][bombX/blockWidth]=FLOOR_P1;
                                break;
                            case 2: floorMap[bombY/blockWidth-i][bombX/blockWidth]=FLOOR_P2;
                                    break;
                        }
                    }
                    else if(countdown==0){
                        if(blockMap[bombY/blockWidth-i][bombX/blockWidth]==BRICK){
                            blockMap[bombY/blockWidth-i][bombX/blockWidth]=9+rand()%(TOTAL_ITEM+1);
                            if(blockMap[bombY/blockWidth-i][bombX/blockWidth]<START_ITEM_REGION)
                                blockMap[bombY/blockWidth-i][bombX/blockWidth]=SMOKE_FRAME_DEFAULT;
                        }
                        else if(blockMap[bombY/blockWidth-i][bombX/blockWidth]==GIFT_BOX){ //100% contain an item
                            blockMap[bombY/blockWidth-i][bombX/blockWidth]=9+rand()%(TOTAL_ITEM+1);
                            if(blockMap[bombY/blockWidth-i][bombX/blockWidth]<START_ITEM_REGION)
                                blockMap[bombY/blockWidth-i][bombX/blockWidth]=I_BOMB_INCRE;
                        }
                        else if(blockMap[bombY/blockWidth-i][bombX/blockWidth]>=START_ITEM_REGION)
                            blockMap[bombY/blockWidth-i][bombX/blockWidth]=SMOKE_FRAME_DEFAULT;
                    }
                    break;
                }
                else if(blockMap[bombY/blockWidth-i][bombX/blockWidth]==BOMB_SET){
                        bombMap[bombY/blockWidth-i][bombX/blockWidth]->setBlast();
                        break;
                }
                else
                    break;
            }
            //explosing right
             for(int i=1;i<=bombLength;i++){
                if(blockMap[bombY/blockWidth][bombX/blockWidth+i]==E_BLOCK || blockMap[bombY/blockWidth][bombX/blockWidth+i]<=SMOKE || blockMap[bombY/blockWidth][bombX/blockWidth+i]==EXPLOSIONING){
                    explosionRender(bombX+i*blockWidth,bombY);
                    if(countdown==19)
                        blockMap[bombY/blockWidth][bombX/blockWidth+i]=EXPLOSIONING;
                    else if(countdown==12){
                        switch(playerID){
                            case 0: break;
                            case 1: floorMap[bombY/blockWidth][bombX/blockWidth+i]=FLOOR_P1;
                                break;
                            case 2: floorMap[bombY/blockWidth][bombX/blockWidth+i]=FLOOR_P2;
                                    break;
                        }
                    }
                    else if(countdown==0)
                        blockMap[bombY/blockWidth][bombX/blockWidth+i]=SMOKE_FRAME_DEFAULT;
                }
                else if(blockMap[bombY/blockWidth][bombX/blockWidth+i]==BRICK || blockMap[bombY/blockWidth][bombX/blockWidth+i]==GIFT_BOX || blockMap[bombY/blockWidth][bombX/blockWidth+i]>=START_ITEM_REGION ){
                    explosionRender(bombX+i*blockWidth,bombY);
                    explosionRender(bombX+i*blockWidth+30,bombY);
                    explosionRender(bombX+i*blockWidth-30,bombY);
                    explosionRender(bombX+i*blockWidth,bombY+30);
                    explosionRender(bombX+i*blockWidth,bombY-30);
                    if(countdown==12){
                        switch(playerID){
                            case 0: break;
                            case 1: floorMap[bombY/blockWidth][bombX/blockWidth+i]=FLOOR_P1;
                                break;
                            case 2: floorMap[bombY/blockWidth][bombX/blockWidth+i]=FLOOR_P2;
                                    break;
                        }
                    }
                    else if(countdown==0){
                        if(blockMap[bombY/blockWidth][bombX/blockWidth+i]==BRICK){
                            blockMap[bombY/blockWidth][bombX/blockWidth+i]=9+rand()%(TOTAL_ITEM+1);
                            if(blockMap[bombY/blockWidth][bombX/blockWidth+i]<START_ITEM_REGION)
                                blockMap[bombY/blockWidth][bombX/blockWidth+i]=SMOKE_FRAME_DEFAULT;
                        }
                        else if(blockMap[bombY/blockWidth][bombX/blockWidth+i]==GIFT_BOX){ //100% contain an item
                            blockMap[bombY/blockWidth][bombX/blockWidth+i]=9+rand()%(TOTAL_ITEM+1);
                            if(blockMap[bombY/blockWidth][bombX/blockWidth+i]<START_ITEM_REGION)
                                blockMap[bombY/blockWidth][bombX/blockWidth+i]=I_BOMB_INCRE;
                        }
                        else if(blockMap[bombY/blockWidth][bombX/blockWidth+i]>=START_ITEM_REGION)
                            blockMap[bombY/blockWidth][bombX/blockWidth+i]=SMOKE_FRAME_DEFAULT;
                    }
                    break;
                }
                else if(blockMap[bombY/blockWidth][bombX/blockWidth+i]==BOMB_SET){
                        bombMap[bombY/blockWidth][bombX/blockWidth+i]->setBlast();
                        break;
                }
                else
                    break;
            }
            //explosing left
            for(int i=1;i<=bombLength;i++){
                if(blockMap[bombY/blockWidth][bombX/blockWidth-i]==E_BLOCK || blockMap[bombY/blockWidth][bombX/blockWidth-i]<=SMOKE || blockMap[bombY/blockWidth][bombX/blockWidth-i]==EXPLOSIONING){
                    explosionRender(bombX-i*blockWidth,bombY);
                    if(countdown==19)
                        blockMap[bombY/blockWidth][bombX/blockWidth-i]=EXPLOSIONING;
                    else if(countdown==12){
                        switch(playerID){
                            case 0: break;
                            case 1: floorMap[bombY/blockWidth][bombX/blockWidth-i]=FLOOR_P1;
                                break;
                            case 2: floorMap[bombY/blockWidth][bombX/blockWidth-i]=FLOOR_P2;
                                    break;
                        }
                    }
                    else if(countdown==0)
                        blockMap[bombY/blockWidth][bombX/blockWidth-i]=SMOKE_FRAME_DEFAULT;
                }
                else if(blockMap[bombY/blockWidth][bombX/blockWidth-i]==BRICK || blockMap[bombY/blockWidth][bombX/blockWidth-i]==GIFT_BOX || blockMap[bombY/blockWidth][bombX/blockWidth-i]>=START_ITEM_REGION){
                    explosionRender(bombX-i*blockWidth,bombY);
                    explosionRender(bombX-i*blockWidth+30,bombY);
                    explosionRender(bombX-i*blockWidth-30,bombY);
                    explosionRender(bombX-i*blockWidth,bombY+30);
                    explosionRender(bombX-i*blockWidth,bombY-30);
                    if(countdown==12){
                        switch(playerID){
                            case 0: break;
                            case 1: floorMap[bombY/blockWidth][bombX/blockWidth-i]=FLOOR_P1;
                                break;
                            case 2: floorMap[bombY/blockWidth][bombX/blockWidth-i]=FLOOR_P2;
                                    break;
                        }
                    }
                    else if(countdown==0){
                        if(blockMap[bombY/blockWidth][bombX/blockWidth-i]==BRICK){
                            blockMap[bombY/blockWidth][bombX/blockWidth-i]=9+rand()%(TOTAL_ITEM+1);
                            if(blockMap[bombY/blockWidth][bombX/blockWidth-i]<START_ITEM_REGION)
                                blockMap[bombY/blockWidth][bombX/blockWidth-i]=SMOKE_FRAME_DEFAULT;
                        }
                        else if(blockMap[bombY/blockWidth][bombX/blockWidth-i]==GIFT_BOX){ //100% contain an item
                            blockMap[bombY/blockWidth][bombX/blockWidth-i]=9+rand()%(TOTAL_ITEM+1);
                            if(blockMap[bombY/blockWidth][bombX/blockWidth-i]<START_ITEM_REGION)
                                blockMap[bombY/blockWidth][bombX/blockWidth-i]=I_BOMB_INCRE;
                        }
                        else if(blockMap[bombY/blockWidth][bombX/blockWidth-i]>=START_ITEM_REGION)
                            blockMap[bombY/blockWidth][bombX/blockWidth-i]=SMOKE_FRAME_DEFAULT;
                    }
                    break;
                }
                else if(blockMap[bombY/blockWidth][bombX/blockWidth-i]==BOMB_SET){
                        bombMap[bombY/blockWidth][bombX/blockWidth-i]->setBlast();
                        break;
                }
                else
                    break;
            }
        }
        else if(countdown<0){
            bombed=false;
            blockMap[bombY/blockWidth][bombX/blockWidth]=SMOKE_FRAME_DEFAULT;
        }
    }
}

bool bomb::checkBombAlive() const{
    return bombed;
}

void bomb::setBlast(){
    if(countdown>=25)
        countdown=24;
}

SDL_Point bomb::getBlock() const{
    return {bombX/blockWidth,bombY/blockWidth};
}

void bomb::explosionRender(int posX,int posY) const{
    SDL_Rect explosion_rect;
    if(countdown>=18)
        explosion_rect=explo_fram1;
    else if(countdown>=16)
        explosion_rect=explo_fram2;
    else if(countdown>=14)
        explosion_rect=explo_fram3;
    else if(countdown>=12)
        explosion_rect=explo_fram4;
    else if(countdown>=10)
        explosion_rect=explo_fram5;
    else if(countdown>=8)
        explosion_rect=explo_fram6;
    else if(countdown>=6)
        explosion_rect=explo_fram7;
    else if(countdown>=4)
        explosion_rect=explo_fram8;
    else if(countdown>=2)
        explosion_rect=explo_fram9;
    else
        explosion_rect=explo_fram10;
    explosion.render(posX,posY,nullptr,&explosion_rect);
}

int bomb::getBombLength() const{
    return bombLength;
}

void bomb::freeBomb(){
    delete robot_bomb1;
    delete xmas_bomb;
    delete skull_bomb;
    delete rose_bomb;
    delete ball_bomb;
    for(int i=0;i<ROW;i++)
        for(int j=0;j<COLUMN;j++)
            delete bombMap[i][j];
}
