#include <gameSound.h>
#include <mapRender.h>
#include <character.h>
#include <bomb.h>
#include <bot.h>
#include <gameController.h>
#include <menuitem.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <SDL_mixer.h>

using namespace std;

int frameRate=0;
bool Exit=false;
bool soundOn=true;

const int SINGLEPLAYER=1;
const int MULTIPLAYER=2;
const int EXIT=3;
const int BACKMENU=4;
const int RETRY=5;
const int TUTORIAL=6;
const int LAND_MODE=7;
const int CLASSIC_MODE=8;

int mainMenu(SDL_Renderer* renderer);
void mainSingleGame(SDL_Renderer* renderer,int CHARACTER);
int chooseSingleCharacter(SDL_Renderer* renderer);
SDL_Point chooseMultiCharacter(SDL_Renderer* renderer);
int chooseMultiMode(SDL_Renderer* renderer);
int renderGameOver(SDL_Renderer* renderer,int winner);
void mainMultiGame_classic(SDL_Renderer* renderer,SDL_Point CHARACTER);
void mainMultiGame_land(SDL_Renderer* renderer,SDL_Point CHARACTER);
void tutorial(SDL_Renderer* renderer);
void pauseGame(SDL_Renderer* renderer,bool& quit,int currentMap);

int main(int argc,char** argv){
    SDL_Window* window=SDL_CreateWindow("CRAZY ARCADE",SDL_WINDOWPOS_CENTERED,30,SCREEN_WIDTH,SCREEN_HEIGHT,0);
    SDL_Surface* iconTitleBar=IMG_Load("asset/icon_titlebar.png");
    SDL_SetWindowIcon(window,iconTitleBar);
    SDL_FreeSurface(iconTitleBar);
    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    srand(time(0));
    TTF_Init();
    initSound();
    bomb::initBomb(renderer);
    bot::initBot(renderer);
    character::initCharacter(renderer);
    initMap(renderer);
    int CHARACTER,mode=0;
    SDL_Point CHARACTER_={0,0};
    while(!Exit){
        if(soundOn && !Mix_PlayingMusic())
            Mix_PlayMusic(menu_music,-1);
        mode=0;
        switch(mainMenu(renderer)){
            case SINGLEPLAYER:  CHARACTER=chooseSingleCharacter(renderer);
                                if(Exit || CHARACTER==0)
                                    break;
                                Mix_HaltMusic();
                                mainSingleGame(renderer,CHARACTER);
                                break;

            case MULTIPLAYER:   while(mode==0 || (CHARACTER_.x==0 && CHARACTER_.y==0)){
                                    mode=chooseMultiMode(renderer);
                                    if(mode==0)
                                        break;
                                    CHARACTER_=chooseMultiCharacter(renderer);
                                    if(CHARACTER_.x==0 && CHARACTER_.y==0 && Exit)
                                        break;
                                }
                                if(Exit || mode==0)
                                    break;
                                Mix_HaltMusic();
                                if(mode==CLASSIC_MODE)
                                    mainMultiGame_classic(renderer,CHARACTER_);
                                else if(mode==LAND_MODE)
                                    mainMultiGame_land(renderer,CHARACTER_);
                                break;

            case EXIT:          break;

            case TUTORIAL:      tutorial(renderer);
                                break;
        }
    }
    destroySound();
    bomb::freeBomb();
    character::freeCharacter();
    Mix_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int mainMenu(SDL_Renderer* renderer){
    gameRender background(renderer);
    background.loadImage("asset/menu/bg.png");

    gameRender title(renderer);
    title.loadImage("asset/menu/title.png");

    gameRender arrow_left(renderer);
    arrow_left.loadImage("asset/menu/arrow_left.png");
    arrow_left.setSize(70,70);

    gameRender arrow_right(renderer);
    arrow_right.loadImage("asset/menu/arrow_right.png");
    arrow_right.setSize(70,70);

    gameRender bSingle(renderer);
    bSingle.loadImage("asset/menu/single.png");
    SDL_Rect bSingleRect={328,345,300,110};
    button singlePlayer(bSingleRect.x,bSingleRect.y,bSingleRect.w,bSingleRect.h);

    gameRender bMulti(renderer);
    bMulti.loadImage("asset/menu/multi.png");
    SDL_Rect bMultiRect={328,475,300,110};
    button multiPlayer(bMultiRect.x,bMultiRect.y,bMultiRect.w,bMultiRect.h);

    gameRender bExit(renderer);
    bExit.loadImage("asset/menu/exit.png");
    SDL_Rect bExitRect={328,595,300,110};
    button exit(bExitRect.x,bExitRect.y,bExitRect.w,bExitRect.h);

    gameRender bSound_on(renderer);
    bSound_on.loadImage("asset/menu/sound_on.png");
    gameRender bSound_off(renderer);
    bSound_off.loadImage("asset/menu/sound_off.png");
    SDL_Rect soundRect={30,SCREEN_HEIGHT-100,71,70};
    button sound(soundRect.x,soundRect.y,soundRect.w,soundRect.h);

    gameRender bTutorial(renderer);
    bTutorial.loadImage("asset/menu/button_tutorial.png");
    SDL_Rect tutorialRect={30,SCREEN_HEIGHT-200,70,70};
    button tutorial(tutorialRect.x,tutorialRect.y,tutorialRect.w,tutorialRect.h);

    SDL_Event event;
    int choice=0,width=0,index=1;
    while(!choice){
        background.render(0,0);
        title.render(0,0);
        bSingle.render(0,0,&bSingleRect);
        bSingle.setBlendTexture(200);
        bMulti.render(0,0,&bMultiRect);
        bMulti.setBlendTexture(200);
        bExit.render(0,0,&bExitRect);
        bExit.setBlendTexture(200);
        bTutorial.render(0,0,&tutorialRect);
        bTutorial.setBlendTexture(200);
        if(soundOn){
            bSound_on.render(0,0,&soundRect);
            bSound_on.setBlendTexture(200);
        }
        else{
            bSound_off.render(0,0,&soundRect);
            bSound_off.setBlendTexture(200);
        }
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                choice=EXIT;
                Exit=true;
            }
            else if(sound.checkArea(event.button.x,event.button.y) && gameRender::waitMouseDown(event)){
                if(!soundOn)
                    Mix_PlayChannel(1,button_press,0);
                if(soundOn){
                    soundOn=false;
                    Mix_HaltMusic();
                }
                else{
                    soundOn=true;
                    Mix_PlayMusic(menu_music,-1);
                }
            }
        }
        if(singlePlayer.checkArea(event.button.x,event.button.y)){
                bSingle.setBlendTexture(255);
                arrow_right.render(bSingleRect.x-70-width,bSingleRect.y+15);
                arrow_left.render(bSingleRect.x+bSingleRect.w+width,bSingleRect.y+15);
                if(gameRender::waitMouseDown(event)){
                    if(soundOn)
                        Mix_PlayChannel(1,button_press,0);
                    choice=SINGLEPLAYER;
                }
        }
        else if(multiPlayer.checkArea(event.button.x,event.button.y)){
                bMulti.setBlendTexture(255);
                arrow_right.render(bMultiRect.x-70-width,bMultiRect.y+15);
                arrow_left.render(bMultiRect.x+bMultiRect.w+width,bMultiRect.y+15);
                if(gameRender::waitMouseDown(event)){
                    if(soundOn)
                        Mix_PlayChannel(1,button_press,0);
                    choice=MULTIPLAYER;
                }
        }
        else if(exit.checkArea(event.button.x,event.button.y)){
                bExit.setBlendTexture(255);
                arrow_right.render(bExitRect.x-70-width,bExitRect.y+15);
                arrow_left.render(bExitRect.x+bExitRect.w+width,bExitRect.y+15);
                if(gameRender::waitMouseDown(event)){
                    choice=EXIT;
                    Exit=true;
                }
        }
        else if(tutorial.checkArea(event.button.x,event.button.y)){
                bTutorial.setBlendTexture(255);
                if(gameRender::waitMouseDown(event)){
                    if(soundOn)
                        Mix_PlayChannel(1,button_press,0);
                    choice=TUTORIAL;
                }
        }
        else if(sound.checkArea(event.button.x,event.button.y)){
                if(soundOn)
                    bSound_on.setBlendTexture(255);
                else
                    bSound_off.setBlendTexture(255);
        }
        width+=index;
        if(width==20 || width==0)
            index=-index;
        SDL_RenderPresent(renderer);
    }
    while(!SDL_PollEvent(&event));//put out all garbage event
    return choice;

}

int chooseSingleCharacter(SDL_Renderer* renderer){
    gameRender background(renderer);
    background.loadImage("asset/menu/bg2.png");

    gameRender text(renderer);
    text.loadFont("asset/font/pac.ttf",40);
    text.renderString(WHITE,"choose character");
    SDL_Rect textRect;
    text.setTextureSize(textRect.w,textRect.h);
    textRect.x=SCREEN_WIDTH/2-textRect.w/2;
    textRect.y=50;

    gameRender Back(renderer);
    Back.loadImage("asset/menu/button_back.png");
    SDL_Rect backRect={10,SCREEN_HEIGHT-90,70,79};
    button bBack(backRect.x,backRect.y,backRect.w,backRect.h);

    gameRender arrow(renderer);
    arrow.loadImage("asset/character/player1_arrow.png");
    SDL_Rect arrowRect={0,170,128,128};

    gameRender boz(renderer);
    boz.loadImage("asset/menu/boz.png");
    SDL_Rect bozRect={10,300,210,230};
    button charBoz(bozRect.x,bozRect.y,bozRect.w,bozRect.h);

    gameRender plunk(renderer);
    plunk.loadImage("asset/menu/plunk.png");
    SDL_Rect plunkRect={240,300,216,230};
    button charPlunk(plunkRect.x,plunkRect.y,plunkRect.w,plunkRect.h);

    gameRender ike(renderer);
    ike.loadImage("asset/menu/ike.png");
    SDL_Rect ikeRect={470,300,210,230};
    button charIke(ikeRect.x,ikeRect.y,ikeRect.w,ikeRect.h);

    gameRender evie(renderer);
    evie.loadImage("asset/menu/evie.png");
    SDL_Rect evieRect={700,300,210,230};
    button charEvie(evieRect.x,evieRect.y,evieRect.w,evieRect.h);
    SDL_Event event;
    int Char=0,width=0,index=1;
    while(!Char){
        background.render(0,0);
        text.render(0,0,&textRect);
        Back.render(0,0,&backRect);
        Back.setBlendTexture(180);
        boz.render(0,0,&bozRect);
        boz.setBlendTexture(180);
        plunk.render(0,0,&plunkRect);
        plunk.setBlendTexture(180);
        ike.render(0,0,&ikeRect);
        ike.setBlendTexture(180);
        evie.render(0,0,&evieRect);
        evie.setBlendTexture(180);
        while(SDL_PollEvent(&event))
            if(event.type==SDL_QUIT){
                Exit=true;
                return 0;
            }
            if(charBoz.checkArea(event.button.x,event.button.y)){
                boz.setBlendTexture(255);
                arrowRect.x=bozRect.x+40;
                arrowRect.y=bozRect.y-150+width;
                arrow.render(0,0,&arrowRect);
                if(gameRender::waitMouseDown(event)){
                    if(soundOn)
                        Mix_PlayChannel(1,button_press,0);
                    Char=BOZ;
                }
            }
            else if(charPlunk.checkArea(event.button.x,event.button.y)){
                plunk.setBlendTexture(255);
                arrowRect.x=plunkRect.x+40;
                arrowRect.y=bozRect.y-150+width;
                arrow.render(0,0,&arrowRect);
                if(gameRender::waitMouseDown(event)){
                    if(soundOn)
                        Mix_PlayChannel(1,button_press,0);
                    Char=PLUNK;
                }
            }
            else if(charIke.checkArea(event.button.x,event.button.y)){
                ike.setBlendTexture(255);
                arrowRect.x=ikeRect.x+40;
                arrowRect.y=bozRect.y-150+width;
                arrow.render(0,0,&arrowRect);
                if(gameRender::waitMouseDown(event)){
                    if(soundOn)
                        Mix_PlayChannel(1,button_press,0);
                    Char=IKE;
                    break;
                }
            }
            else if(charEvie.checkArea(event.button.x,event.button.y)){
                evie.setBlendTexture(255);
                arrowRect.x=evieRect.x+40;
                arrowRect.y=bozRect.y-150+width;
                arrow.render(0,0,&arrowRect);
                if(gameRender::waitMouseDown(event)){
                    if(soundOn)
                        Mix_PlayChannel(1,button_press,0);
                    Char=EVIE;
                }
            }
            else if(bBack.checkArea(event.button.x,event.button.y)){
                Back.setBlendTexture(255);
                if(gameRender::waitMouseDown(event)){
                    if(soundOn)
                        Mix_PlayChannel(1,button_press,0);
                    return 0;
                }
            }
            width+=index;
            if(width==30|| width==0)
                index=-index;
            SDL_RenderPresent(renderer);
    }
    return Char;


}

SDL_Point chooseMultiCharacter(SDL_Renderer* renderer){
    gameRender background(renderer);
    background.loadImage("asset/menu/bg2.png");

    gameRender text(renderer);
    text.loadFont("asset/font/pac.ttf",40);
    text.renderString(WHITE,"choose character");
    SDL_Rect textRect;
    text.setTextureSize(textRect.w,textRect.h);
    textRect.x=SCREEN_WIDTH/2-textRect.w/2;
    textRect.y=50;

    gameRender Back(renderer);
    Back.loadImage("asset/menu/button_back.png");
    SDL_Rect backRect={10,SCREEN_HEIGHT-90,70,79};
    button bBack(backRect.x,backRect.y,backRect.w,backRect.h);

    gameRender arrow(renderer);
    arrow.loadImage("asset/character/player1_arrow.png");
    SDL_Rect arrowRect={0,170,128,128};

    gameRender boz(renderer);
    boz.loadImage("asset/menu/boz.png");
    SDL_Rect bozRect={10,300,210,230};
    button charBoz(bozRect.x,bozRect.y,bozRect.w,bozRect.h);

    gameRender plunk(renderer);
    plunk.loadImage("asset/menu/plunk.png");
    SDL_Rect plunkRect={240,300,216,230};
    button charPlunk(plunkRect.x,plunkRect.y,plunkRect.w,plunkRect.h);

    gameRender ike(renderer);
    ike.loadImage("asset/menu/ike.png");
    SDL_Rect ikeRect={470,300,210,230};
    button charIke(ikeRect.x,ikeRect.y,ikeRect.w,ikeRect.h);

    gameRender evie(renderer);
    evie.loadImage("asset/menu/evie.png");
    SDL_Rect evieRect={700,300,210,230};
    button charEvie(evieRect.x,evieRect.y,evieRect.w,evieRect.h);
    SDL_Event event;
    int Char1=0,width=0,index=1;
    //choose character 1
    while(!Char1){
        background.render(0,0);
        text.render(0,0,&textRect);
        Back.render(0,0,&backRect);
        Back.setBlendTexture(180);
        boz.render(0,0,&bozRect);
        boz.setBlendTexture(180);
        plunk.render(0,0,&plunkRect);
        plunk.setBlendTexture(180);
        ike.render(0,0,&ikeRect);
        ike.setBlendTexture(180);
        evie.render(0,0,&evieRect);
        evie.setBlendTexture(180);
        while(SDL_PollEvent(&event))
            if(event.type==SDL_QUIT){
                Exit=true;
                return {0,0};
            }
        if(charBoz.checkArea(event.button.x,event.button.y)){
            boz.setBlendTexture(255);
            arrowRect.x=bozRect.x+40;
            arrowRect.y=bozRect.y-150+width;
            arrow.render(0,0,&arrowRect);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                        Mix_PlayChannel(1,confirm_sound,0);
                Char1=BOZ;
            }
        }
        else if(charPlunk.checkArea(event.button.x,event.button.y)){
            plunk.setBlendTexture(255);
            arrowRect.x=plunkRect.x+40;
            arrowRect.y=bozRect.y-150+width;
            arrow.render(0,0,&arrowRect);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                Char1=PLUNK;
            }
        }
        else if(charIke.checkArea(event.button.x,event.button.y)){
            ike.setBlendTexture(255);
            arrowRect.x=ikeRect.x+40;
            arrowRect.y=bozRect.y-150+width;
            arrow.render(0,0,&arrowRect);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                Char1=IKE;
            }
        }
        else if(charEvie.checkArea(event.button.x,event.button.y)){
            evie.setBlendTexture(255);
            arrowRect.x=evieRect.x+40;
            arrowRect.y=bozRect.y-150+width;
            arrow.render(0,0,&arrowRect);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                Char1=EVIE;
            }
        }
        else if(bBack.checkArea(event.button.x,event.button.y)){
            Back.setBlendTexture(255);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,button_press,0);
                return {0,0};
            }
        }
        width+=index;
        if(width==30|| width==0)
            index=-index;
        SDL_RenderPresent(renderer);
    }
    //choose charater 2
    arrow.loadImage("asset/character/player2_arrow.png");
    int Char2=0;
    while(!Char2 || Char2==Char1){
        background.render(0,0);
        text.render(0,0,&textRect);
        Back.render(0,0,&backRect);
        Back.setBlendTexture(180);
        boz.render(0,0,&bozRect);
        boz.setBlendTexture(180);
        plunk.render(0,0,&plunkRect);
        plunk.setBlendTexture(180);
        ike.render(0,0,&ikeRect);
        ike.setBlendTexture(180);
        evie.render(0,0,&evieRect);
        evie.setBlendTexture(180);
        while(SDL_PollEvent(&event))
            if(event.type==SDL_QUIT){
                Exit=true;
                return {0,0};
            }
        if(charBoz.checkArea(event.button.x,event.button.y) && Char1!=BOZ){
            boz.setBlendTexture(255);
            arrowRect.x=bozRect.x+40;
            arrowRect.y=bozRect.y-150+width;
            arrow.render(0,0,&arrowRect);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                Char2=BOZ;
            }
        }
        else if(charPlunk.checkArea(event.button.x,event.button.y)&& Char1!=PLUNK){
            plunk.setBlendTexture(255);
            arrowRect.x=plunkRect.x+40;
            arrowRect.y=bozRect.y-150+width;
            arrow.render(0,0,&arrowRect);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                Char2=PLUNK;
            }
        }
        else if(charIke.checkArea(event.button.x,event.button.y)&& Char1!=IKE){
            ike.setBlendTexture(255);
            arrowRect.x=ikeRect.x+40;
            arrowRect.y=bozRect.y-150+width;
            arrow.render(0,0,&arrowRect);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                Char2=IKE;
            }
        }
        else if(charEvie.checkArea(event.button.x,event.button.y) && Char1!=EVIE){
            evie.setBlendTexture(255);
            arrowRect.x=evieRect.x+40;
            arrowRect.y=bozRect.y-150+width;
            arrow.render(0,0,&arrowRect);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                Char2=EVIE;
            }
        }
        else if(bBack.checkArea(event.button.x,event.button.y)){
            Back.setBlendTexture(255);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,button_press,0);
                return {0,0};
            }
        }
        width+=index;
        if(width==30|| width==0)
            index=-index;
        SDL_RenderPresent(renderer);
    }
    return {Char1,Char2};
}

int chooseMultiMode(SDL_Renderer* renderer){
     gameRender background(renderer);
    background.loadImage("asset/menu/bg2.png");

    gameRender text(renderer);
    text.loadFont("asset/font/pac.ttf",40);
    text.renderString(WHITE,"choose mode");
    SDL_Rect textRect;
    text.setTextureSize(textRect.w,textRect.h);
    textRect.x=SCREEN_WIDTH/2-textRect.w/2;
    textRect.y=50;

    gameRender Back(renderer);
    Back.loadImage("asset/menu/button_back.png");
    SDL_Rect backRect={10,SCREEN_HEIGHT-90,70,79};
    button bBack(backRect.x,backRect.y,backRect.w,backRect.h);

    gameRender landMode(renderer);
    landMode.loadImage("asset/menu/landmode.png");
    SDL_Rect landModeRect={SCREEN_WIDTH/2-731/2,400,731,220};
    button bLandMode(landModeRect.x,landModeRect.y,landModeRect.w,landModeRect.h);

    gameRender classicMode(renderer);
    classicMode.loadImage("asset/menu/classic.png");
    SDL_Rect classicModeRect={SCREEN_WIDTH/2-731/2,150,731,220};
    button bClassicMode(classicModeRect.x,classicModeRect.y,classicModeRect.w,classicModeRect.h);
    SDL_Event event;
    int mode=0;
    while(!mode){
        background.render(0,0);
        text.render(0,0,&textRect);
        Back.render(0,0,&backRect);
        Back.setBlendTexture(180);
        landMode.render(0,0,&landModeRect);
        landMode.setBlendTexture(180);
        classicMode.render(0,0,&classicModeRect);
        classicMode.setBlendTexture(180);
        while(SDL_PollEvent(&event))
            if(event.type==SDL_QUIT){
                Exit=true;
                return 0;
            }
        if(bLandMode.checkArea(event.button.x,event.button.y)){
            landMode.setBlendTexture(255);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                mode=LAND_MODE;
            }
        }
        else if(bClassicMode.checkArea(event.button.x,event.button.y)){
            classicMode.setBlendTexture(255);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,confirm_sound,0);
                mode=CLASSIC_MODE;
            }
        }
        else if(bBack.checkArea(event.button.x,event.button.y)){
            Back.setBlendTexture(255);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,button_press,0);
                return 0;
            }
        }
        SDL_RenderPresent(renderer);
    }
    while(!SDL_PollEvent(&event));
    return mode;

}

void mainSingleGame(SDL_Renderer* renderer,int CHARACTER){
    character player1(CHARACTER,0,TOP_LEFT);
    bot bot1,bot2,bot3,bot4;
    gameRender pause(renderer);
    pause.loadImage("asset/menu/pause_button.png");
    SDL_Rect bPauseRect={SCREEN_WIDTH-50,10,32,32};
    button bPause(bPauseRect.x,bPauseRect.y,bPauseRect.w,bPauseRect.h);
    int timeUP=false,round_count=0,level=1;
    SDL_Event event;
    bool quit=false,lock_on=false;
    SDL_Point lockBlock,playerBlock;
    SDL_Rect default_viewport={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    if(soundOn)
        Mix_PlayChannel(1,start_game,0);
    do{
        SDL_RenderSetViewport(renderer,&default_viewport);
        loadMap(level);
        player1=character(CHARACTER,0,TOP_LEFT);
        bot1=bot();
        bot2=bot();
        bot3=bot();
        bot4=bot();
        for(int i=1;i<ROW-1;i++)
            for(int j=1;j<COLUMN-1;j++){
                if(blockMap[i][j]==BOT1_POS){
                    bot1.setPosition(i,j);
                    blockMap[i][j]=1;
                }
                else if(blockMap[i][j]==BOT2_POS){
                    bot2.setPosition(i,j);
                    blockMap[i][j]=1;
                }
                else if(blockMap[i][j]==BOT3_POS){
                    bot3.setPosition(i,j);
                    blockMap[i][j]=1;
                }
                else if(blockMap[i][j]==BOT4_POS){
                    bot4.setPosition(i,j);
                    blockMap[i][j]=1;
                }
            }
        do{
            lockBlock.x=1+rand()%(COLUMN-2);
            lockBlock.y=1+rand()%(ROW-2);
        }while(blockMap[lockBlock.y][lockBlock.x]!=BRICK && blockMap[lockBlock.y][lockBlock.x]!=GIFT_BOX);
        while(!quit && level==1){
            while(SDL_PollEvent(&event))
                if(event.type==SDL_QUIT){
                    Exit=true;
                    return ;
                }
                else if((event.type==SDL_MOUSEBUTTONDOWN && bPause.checkArea(event.button.x,event.button.y))|| (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
                    pauseGame(renderer,quit,level);
            renderFloor(lock_on,lockBlock);
            renderBlock();
            pause.render(0,0,&bPauseRect);
            player1.renderOpeningScene(1);
            bot1.renderBot(player1,lock_on,lockBlock);
            bot2.renderBot(player1,lock_on,lockBlock);
            bot3.renderBot(player1,lock_on,lockBlock);
            bot4.renderBot(player1,lock_on,lockBlock);
            SDL_RenderPresent(renderer);
            frameRate++;
            if(frameRate>=24){
                round_count++;
                frameRate=0;
            }
            if(round_count==6)
                break;
        }
        frameRate=0;
        round_count=0;
        lock_on=false;
        resetTime();
        while(!quit){
            while(SDL_PollEvent(&event)){
                if(event.type==SDL_QUIT){
                    Exit=true;
                    return;
                }
                else if((event.type==SDL_MOUSEBUTTONDOWN && bPause.checkArea(event.button.x,event.button.y))|| (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
                    pauseGame(renderer,quit,level);
                if(!player1.getDeadState())
                    gameControl1P(event,&player1);
            }
            SDL_RenderClear(renderer);
            renderFloor(lock_on,lockBlock);
            player1.bombCountdown();
            renderBlock();
            pause.render(0,0,&bPauseRect);
            timeUP=renderTime();
            player1.renderPlayer();
            if(!player1.getDeadState() && !timeUP)
                player1.checkDead();
            else if(player1.getDeadState() || timeUP){
                int choice=renderGameOver(renderer,0);
                if(choice==RETRY)
                    level=0;
                else if(choice==BACKMENU)
                    quit=true;
                else if(choice==EXIT)
                    return;
                break;
            }
            bot1.renderBot(player1,lock_on,lockBlock);
            if(!bot1.getDeadState())
                bot1.checkDead();
            bot2.renderBot(player1,lock_on,lockBlock);
            if(!bot2.getDeadState())
                bot2.checkDead();
            bot3.renderBot(player1,lock_on,lockBlock);
            if(!bot3.getDeadState())
                bot3.checkDead();
            bot4.renderBot(player1,lock_on,lockBlock);
            if(!bot4.getDeadState())
                bot4.checkDead();
            playerBlock=player1.getBlock();
            if(checkSinglePWIN(playerBlock,lockBlock,lock_on))
                break;
            SDL_RenderPresent(renderer);
            frameRate++;
            if(frameRate>=24)
                frameRate=0;
        }
        Mix_HaltMusic();
        level++;
        if(level==5 && !quit){
            int choice=renderGameOver(renderer,3);
            if(choice==RETRY)
                level=0;
            else if(choice==BACKMENU)
                quit=true;
            break;
        }
    }while(!quit && level<=4);
}

void mainMultiGame_classic(SDL_Renderer* renderer,SDL_Point CHARACTER){
    character player1(CHARACTER.x,0,TOP_LEFT),player2(CHARACTER.y,0,BOT_RIGHT);
    gameRender pause(renderer);
    pause.loadImage("asset/menu/pause_button.png");
    SDL_Rect bPauseRect={SCREEN_WIDTH-50,10,32,32};
    button bPause(bPauseRect.x,bPauseRect.y,bPauseRect.w,bPauseRect.h);

    int round_count=0;
    SDL_Event event;
    bool quit=false;
    SDL_Rect default_viewport={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    if(soundOn)
        Mix_PlayChannel(1,start_game,0);
    do{
        SDL_RenderSetViewport(renderer,&default_viewport);
        int randomMap=M_TOWN+rand()%4;
        loadMap(randomMap);
        player1=character(CHARACTER.x,0,TOP_LEFT);
        player2=character(CHARACTER.y,0,BOT_RIGHT);
        switch(rand()%4){
            case 0:     player1.setPosition(TOP_LEFT);
                        player2.setPosition(BOT_RIGHT);
                        break;
            case 1:     player1.setPosition(TOP_RIGHT);
                        player2.setPosition(BOT_LEFT);
                        break;
            case 2:     player1.setPosition(BOT_LEFT);
                        player2.setPosition(TOP_RIGHT);
                        break;
            case 3:     player1.setPosition(BOT_RIGHT);
                        player2.setPosition(TOP_LEFT);
                        break;
        }
        while(!quit){
            while(SDL_PollEvent(&event))
                if(event.type==SDL_QUIT){
                    Exit=true;
                    return;
                }
                else if((event.type==SDL_MOUSEBUTTONDOWN && bPause.checkArea(event.button.x,event.button.y))|| (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
                    pauseGame(renderer,quit,randomMap);
            renderFloor();
            renderBlock();
            pause.render(0,0,&bPauseRect);
            player1.renderOpeningScene(1);
            player2.renderOpeningScene(2);
            SDL_RenderPresent(renderer);
            frameRate++;
            if(frameRate>=24){
                round_count++;
                frameRate=0;
            }
            if(round_count==6)
                break;
        }
        frameRate=0;
        round_count=0;
        while(!quit){
            while(SDL_PollEvent(&event)){
                if(event.type==SDL_QUIT){
                    Exit=true;
                    return;
                }
                else if((event.type==SDL_MOUSEBUTTONDOWN && bPause.checkArea(event.button.x,event.button.y))|| (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
                    pauseGame(renderer,quit,randomMap);
                if(!player1.getDeadState() && !player2.getDeadState())
                    gameControl2P(event,&player1,&player2);
            }
            SDL_RenderClear(renderer);
            renderFloor();
            player1.bombCountdown();
            player2.bombCountdown();
            renderBlock();
            pause.render(0,0,&bPauseRect);
            player1.renderPlayer();
            player2.renderPlayer();
            if(!player1.getDeadState() && !player2.getDeadState()){
                player1.checkDead();
                player2.checkDead();
            }
            else{
                int choice;
                if(player1.getDeadState())
                    choice=renderGameOver(renderer,2);
                else
                    choice=renderGameOver(renderer,1);
                if(choice==RETRY)
                    break;
                else if(choice==BACKMENU)
                    quit=true;
                else if(choice==EXIT){
                    Exit=true;
                    return;
                }
                break;
            }
            SDL_RenderPresent(renderer);
            frameRate++;
            if(frameRate>=24)
                frameRate=0;
            }
            Mix_HaltMusic();
    }while(!quit);
}

void mainMultiGame_land(SDL_Renderer* renderer,SDL_Point CHARACTER){
    character player1(CHARACTER.x,1,TOP_LEFT),player2(CHARACTER.y,2,BOT_RIGHT);
    gameRender pause(renderer);
    pause.loadImage("asset/menu/pause_button.png");
    SDL_Rect bPauseRect={SCREEN_WIDTH-50,10,32,32};
    button bPause(bPauseRect.x,bPauseRect.y,bPauseRect.w,bPauseRect.h);
    int round_count=0,randomMap,winner;
    SDL_Event event;
    bool quit=false,timeUp=false;
    if(soundOn)
        Mix_PlayChannel(1,start_game,0);
    do{
        resetFloor();
        randomMap=L_MAP1+rand()%2;
        loadMap(randomMap);
        player1=character(CHARACTER.x,1,TOP_LEFT);
        player2=character(CHARACTER.y,2,BOT_RIGHT);
        switch(rand()%4){
            case 0:     player1.setPosition(TOP_LEFT);
                        player2.setPosition(BOT_RIGHT);
                        break;
            case 1:     player1.setPosition(TOP_RIGHT);
                        player2.setPosition(BOT_LEFT);
                        break;
            case 2:     player1.setPosition(BOT_LEFT);
                        player2.setPosition(TOP_RIGHT);
                        break;
            case 3:     player1.setPosition(BOT_RIGHT);
                        player2.setPosition(TOP_LEFT);
                        break;
        }
        while(!quit){
            while(SDL_PollEvent(&event))
                if(event.type==SDL_QUIT){
                    Exit=true;
                    return;
                }
                else if((event.type==SDL_MOUSEBUTTONDOWN && bPause.checkArea(event.button.x,event.button.y))|| (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
                    pauseGame(renderer,quit,randomMap);
            renderFloor_landMode();
            renderBlock();
            pause.render(0,0,&bPauseRect);
            winner=renderLandOcupied();
            player1.renderOpeningScene(1);
            player2.renderOpeningScene(2);
            SDL_RenderPresent(renderer);
            frameRate++;
            if(frameRate>=24){
                round_count++;
                frameRate=0;
            }
            if(round_count==6)
                break;
        }
        frameRate=0;
        resetTime();
        round_count=0;
        while(!quit){
            while(SDL_PollEvent(&event)){
                if(event.type==SDL_QUIT){
                    Exit=true;
                    return;
                }
                else if((event.type==SDL_MOUSEBUTTONDOWN && bPause.checkArea(event.button.x,event.button.y))|| (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE))
                    pauseGame(renderer,quit,randomMap);
                    gameControl2P(event,&player1,&player2);
            }
            SDL_RenderClear(renderer);
            renderFloor_landMode();
            player1.bombCountdown();
            player2.bombCountdown();
            renderBlock();
            pause.render(0,0,&bPauseRect);
            winner=renderLandOcupied();
            timeUp=renderTime();
            player1.renderPlayer();
            player2.renderPlayer();
            if(!player1.getDeadState())
                player1.checkDead();
            else
                player1.reviveCharacter();
            if(!player2.getDeadState())
                player2.checkDead();
            else
                player2.reviveCharacter();
            if(timeUp){
                int choice=renderGameOver(renderer,winner+1);
                if(choice==RETRY)
                    break;
                else if(choice==BACKMENU)
                    quit=true;
                else if(choice==EXIT){
                    Exit=true;
                    return;
                }
                break;
            }
            SDL_RenderPresent(renderer);
            frameRate++;
            if(frameRate>=24)
                frameRate=0;
            }
            Mix_HaltMusic();
    }while(!quit);
}

int renderGameOver(SDL_Renderer* renderer,int winner){
    Mix_HaltMusic();
    gameRender window_bg(renderer);
    window_bg.loadImage("asset/menu/window_bg.png");
    window_bg.setSize(321,200);

    gameRender gameOver(renderer);
    gameOver.loadFont("asset/font/minecraft.otf",35);
    SDL_Rect gameOverRect;

    gameRender sound_on(renderer);
    sound_on.loadImage("asset/menu/sound_on.png");
    gameRender sound_off(renderer);
    sound_off.loadImage("asset/menu/sound_off.png");
    SDL_Rect soundRect={450,SCREEN_HEIGHT/2-30,71,70};
    button bSound(soundRect.x,soundRect.y,soundRect.w,soundRect.h);

    gameRender backMenu(renderer);
    backMenu.loadImage("asset/menu/close_button.png");
    SDL_Rect backRect={340,SCREEN_HEIGHT/2-30,71,70};
    button bBack(backRect.x,backRect.y,backRect.w,backRect.h);

    gameRender retry(renderer);
    retry.loadImage("asset/menu/retry.png");
    SDL_Rect retryRect={560,SCREEN_HEIGHT/2-30,71,70};
    button bRetry(retryRect.x,retryRect.y,retryRect.w,retryRect.h);

    switch(winner){
        case 0: gameOver.renderString(BLACK,"GAME OVER");
                if(soundOn)
                    Mix_PlayChannel(-1,levelFail_sound,0);
                break;
        case 1: gameOver.renderString(GREEN,"PLAYER 1 WIN");
                if(soundOn)
                    Mix_PlayChannel(-1,win_sound,0);
                break;
        case 2: gameOver.renderString(RED,"PLAYER 2 WIN");
                if(soundOn)
                    Mix_PlayChannel(-1,win_sound,0);
                break;
        case 3: gameOver.renderString(BLUE,"YOU WIN");
                if(soundOn)
                    Mix_PlayChannel(-1,win_sound,0);
                break;
    }
    gameOver.setTextureSize(gameOverRect.w,gameOverRect.h);
    gameOverRect.x=SCREEN_WIDTH/2-gameOverRect.w/2+5;
    gameOverRect.y=SCREEN_HEIGHT/2-gameOverRect.h/2-95;
    int choice=0;
    SDL_Rect default_viewport={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_Event event;
    while(!choice){
        SDL_RenderSetViewport(renderer,&default_viewport);
        window_bg.render(320,SCREEN_HEIGHT/2-140);
        gameOver.render(0,0,&gameOverRect);
        backMenu.render(0,0,&backRect);
        if(soundOn)
            sound_on.render(0,0,&soundRect);
        else
            sound_off.render(0,0,&soundRect);
        retry.render(0,0,&retryRect);
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                choice=EXIT;
                Exit=true;
            }
            else if(bBack.checkArea(event.button.x,event.button.y)){
                if(gameRender::waitMouseDown(event))
                    choice=BACKMENU;
            }
            else if(bSound.checkArea(event.button.x,event.button.y)){
                if(gameRender::waitMouseDown(event)){
                    if(soundOn){
                        Mix_HaltChannel(-1);
                        soundOn=false;
                    }
                    else{
                        if(winner==0)
                            Mix_PlayChannel(-1,levelFail_sound,0);
                        else
                            Mix_PlayChannel(-1,win_sound,0);
                        soundOn=true;
                    }
                }
            }
            else if(bRetry.checkArea(event.button.x,event.button.y)){
                if(gameRender::waitMouseDown(event))
                    choice=RETRY;
            }
        }
        SDL_RenderPresent(renderer);
    }
    Mix_HaltChannel(-1);
    return choice;
}

void pauseGame(SDL_Renderer* renderer,bool& quit,int currentMap){
    int timeBeforePause=SDL_GetTicks();

    gameRender window_bg(renderer);
    window_bg.loadImage("asset/menu/window_bg.png");
    window_bg.setSize(321,200);

    gameRender paused(renderer);
    paused.loadFont("asset/font/minecraft.otf",45);
    paused.renderString(RED,"PAUSED");
    SDL_Rect pausedRect;
    paused.setTextureSize(pausedRect.w,pausedRect.h);
    pausedRect.x=SCREEN_WIDTH/2-pausedRect.w/2+5;
    pausedRect.y=SCREEN_HEIGHT/2-pausedRect.h/2-95;

    gameRender sound_on(renderer);
    sound_on.loadImage("asset/menu/sound_on.png");
    gameRender sound_off(renderer);
    sound_off.loadImage("asset/menu/sound_off.png");
    SDL_Rect soundRect={450,SCREEN_HEIGHT/2-30,71,70};
    button bSound(soundRect.x,soundRect.y,soundRect.w,soundRect.h);

    gameRender backMenu(renderer);
    backMenu.loadImage("asset/menu/close_button.png");
    SDL_Rect backRect={340,SCREEN_HEIGHT/2-30,71,70};
    button bBack(backRect.x,backRect.y,backRect.w,backRect.h);

    gameRender Continue(renderer);
    Continue.loadImage("asset/menu/play_button.png");
    SDL_Rect continueRect={560,SCREEN_HEIGHT/2-30,71,70};
    button bContinue(continueRect.x,continueRect.y,continueRect.w,continueRect.h);
    SDL_Event event;
    bool pausing=true;
    while(pausing){
        window_bg.render(320,SCREEN_HEIGHT/2-140);
        paused.render(0,0,&pausedRect);
        backMenu.render(0,0,&backRect);
        if(soundOn)
            sound_on.render(0,0,&soundRect);
        else
            sound_off.render(0,0,&soundRect);
        Continue.render(0,0,&continueRect);
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                quit=true;
                Exit=true;
                pausing=false;
            }
            else if(bBack.checkArea(event.button.x,event.button.y)){
                if(gameRender::waitMouseDown(event)){
                    quit=true;
                    pausing=false;
                }
            }
            else if(bSound.checkArea(event.button.x,event.button.y)){
                if(gameRender::waitMouseDown(event)){
                    if(soundOn){
                        Mix_HaltMusic();
                        soundOn=false;
                    }
                    else{
                        if(currentMap==S_TOWN || currentMap==M_TOWN || currentMap==L_MAP1 || currentMap==L_MAP2)
                            Mix_PlayMusic(town_music,-1);
                        else if(currentMap==S_DESERT || currentMap==M_DESERT)
                            Mix_PlayMusic(desert_music,-1);
                        else if(currentMap==S_XMAS || currentMap==M_XMAS)
                            Mix_PlayMusic(xmas_music,-1);
                        else if(currentMap==S_UNDERWATER || currentMap==M_UNDERWATER)
                            Mix_PlayMusic(underwater_music,-1);
                        soundOn=true;
                    }
                }
            }
            else if((bContinue.checkArea(event.button.x,event.button.y)&&gameRender::waitMouseDown(event)) || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE) )
                    pausing=false;
        }
        SDL_RenderPresent(renderer);
    }
    pauseTime(timeBeforePause);
}

void tutorial(SDL_Renderer* renderer){
    gameRender background(renderer);
    background.loadImage("asset/menu/bg.png");

    gameRender control_tut(renderer);
    control_tut.loadImage("asset/menu/control_tutorial.png");
    control_tut.setBlendTexture(230);
    SDL_Rect tutRect={SCREEN_WIDTH/2-666/2,SCREEN_HEIGHT/2-512/2,666,512};

    gameRender item_tut(renderer);
    item_tut.loadImage("asset/menu/item_tutorial.png");
    item_tut.setBlendTexture(230);

    gameRender Previous(renderer);
    Previous.loadImage("asset/menu/button_previous.png");
    SDL_Rect previousRect={tutRect.x-80,SCREEN_HEIGHT/2-50/2,50,50};
    button bPrevious(previousRect.x,previousRect.y,previousRect.w,previousRect.h);

    gameRender Next(renderer);
    Next.loadImage("asset/menu/button_next.png");
    SDL_Rect nextRect={tutRect.x+tutRect.w+20,SCREEN_HEIGHT/2-50/2,50,50};
    button bNext(nextRect.x,nextRect.y,nextRect.w,nextRect.h);

    gameRender Back(renderer);
    Back.loadImage("asset/menu/button_back.png");
    SDL_Rect backRect={10,SCREEN_HEIGHT-90,70,79};
    button bBack(backRect.x,backRect.y,backRect.w,backRect.h);

    SDL_Event event;
    int page=1;
    bool quit=false;
    while(!quit){
        background.render(0,0);
        if(page==1)
            control_tut.render(0,0,&tutRect);
        else
            item_tut.render(0,0,&tutRect);
        Back.render(0,0,&backRect);
        Back.setBlendTexture(200);
        if(page==1){
            Next.render(0,0,&nextRect);
            Next.setBlendTexture(200);
        }
        else{
            Previous.render(0,0,&previousRect);
            Previous.setBlendTexture(200);
        }
        while(SDL_PollEvent(&event))
            if(event.type==SDL_QUIT){
                quit=true;
                Exit=true;
            }
        if(bBack.checkArea(event.button.x,event.button.y)){
            Back.setBlendTexture(255);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,button_press,0);
                quit=true;
            }
        }
        if(bNext.checkArea(event.button.x,event.button.y) && page==1){
            Next.setBlendTexture(255);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,button_press,0);
                page=2;
            }
        }
        if(bPrevious.checkArea(event.button.x,event.button.y) && page==2){
            Previous.setBlendTexture(255);
            if(gameRender::waitMouseDown(event)){
                if(soundOn)
                    Mix_PlayChannel(1,button_press,0);
                page=1;
            }
        }
        SDL_RenderPresent(renderer);
    }

}
