#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <cstdlib>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "gameRender.h"

gameRender::gameRender(SDL_Renderer* renderer_){
    renderer=renderer_;
    texture=nullptr;
    font=nullptr;
    width=SCREEN_WIDTH;
    height=SCREEN_HEIGHT;
}

gameRender::~gameRender(){
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void gameRender::setRenderer(SDL_Renderer* renderer){
    this->renderer=renderer;
}

void gameRender::render(int x,int y,SDL_Rect* dstrect,SDL_Rect* srcrect){
    if(dstrect==nullptr && srcrect==nullptr){
        SDL_Rect temp={x,y,width,height};
        SDL_RenderCopy(renderer,texture,nullptr,&temp);
    }
    else if(dstrect==nullptr){
        SDL_Rect temp={x,y,width,height};
        SDL_RenderCopy(renderer,texture,srcrect,&temp);
    }
    else if(srcrect==nullptr)
        SDL_RenderCopy(renderer,texture,nullptr,dstrect);
    else
        SDL_RenderCopy(renderer,texture,srcrect,dstrect);
}

void gameRender::loadImage(std::string path,SDL_Color* colorKey ){
    if(texture!=nullptr)
        SDL_DestroyTexture(texture);
    SDL_Surface* surface=nullptr;
    bool colorKeyuse=false;
    if(colorKey!=nullptr)
        colorKeyuse=true;
    surface=IMG_Load(path.c_str());
    if(surface==nullptr)
        std::cout<<"LOADED FAILED"<<" "<<IMG_GetError()<<std::endl;
    else{
        if(colorKeyuse){
            SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format,colorKey->r,colorKey->g,colorKey->b));
            texture=SDL_CreateTextureFromSurface(renderer,surface);
        }
        else
            texture=SDL_CreateTextureFromSurface(renderer,surface);
    }
    SDL_FreeSurface(surface);
}

int gameRender::getWidth(){
    return width;
}

int gameRender::getHeight(){
    return height;
}

std::string gameRender::intToString(int number)
{
    std::stringstream ss;
    ss << number;
    std::string str = ss.str();
    return str;
}

bool gameRender::waitMouseDown(SDL_Event &event){
    if(event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
        return true;
    return false;
}

void gameRender::renderString(SDL_Color color,std::string s){
     std::string  showstring=s;
     SDL_Surface* surface=nullptr;
     if(texture!=nullptr)
        SDL_DestroyTexture(texture); // avoid memory leak
     surface=TTF_RenderText_Solid(font,showstring.c_str(),color);
     texture=SDL_CreateTextureFromSurface(renderer,surface);
     SDL_FreeSurface(surface);
}

void  gameRender::setTextureSize(int& widthx,int& heightx){
    SDL_QueryTexture(texture,NULL,NULL,&widthx,&heightx);
}

void gameRender::setSize(int width,int height){
    this->width=width;
    this->height=height;
}

void gameRender::setBlendTexture(Uint8 alpha){
    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture,alpha);
}

void gameRender::loadFont(std::string path,int size){
    if(font!=nullptr)
         TTF_CloseFont(font);
     font=TTF_OpenFont(path.c_str(),size);
     if(font==nullptr)
        std::cout<<"Failed to load font"<<TTF_GetError()<<std::endl;
}
