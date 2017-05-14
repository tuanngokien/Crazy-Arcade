#ifndef GAMERENDER_H
#define GAMERENDER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

const SDL_Color WHITE={255,255,255,255};
const SDL_Color BLACK={0,0,0,255};
const SDL_Color BLUE={0,0,255,255};
const SDL_Color CYAN = {0,255,255};
const SDL_Color RED={255,0,0,255};
const SDL_Color GREEN={0,255,0,255};

const int SCREEN_HEIGHT=704;
const int SCREEN_WIDTH=960;

const int ROW=11;
const int COLUMN=15;
const int blockWidth=64;


class gameRender{
private:
    int width;
    int height;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    TTF_Font *font;

public:
    gameRender(SDL_Renderer* renderer_=nullptr);
    ~gameRender();
    int getWidth();
    int getHeight();
    void setSize(int width,int height);
    void setRenderer(SDL_Renderer* renderer);
    void render(int x,int y,SDL_Rect* dstrect =nullptr,SDL_Rect* srcrect =nullptr);
    void loadImage(std::string path,SDL_Color* colorKey =nullptr);
    void renderString(SDL_Color color,std::string s);
    static bool waitMouseDown(SDL_Event &event);
    void setTextureSize(int& width,int& height);
    static std::string intToString(int number);
    void setBlendTexture(Uint8 alpha);
    void loadFont(std::string path,int size);
};


#endif //GAMERENDER_H
