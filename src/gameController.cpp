#include <gameController.h>
#include <cstdlib>

extern bomb* bombMap[ROW][COLUMN];
int AIMap[ROW][COLUMN];
const int DANGER=5;
void gameControl1P(SDL_Event& event,character* player1){
    if(event.type==SDL_KEYDOWN && event.key.repeat==0)
        switch(event.key.keysym.sym){
            case SDLK_UP:       player1->setVelocityY(-player1->getCurrentVel());
                                break;
            case SDLK_DOWN:     player1->setVelocityY(player1->getCurrentVel());
                                break;
            case SDLK_LEFT:     player1->setVelocityX(-player1->getCurrentVel());
                                break;
            case SDLK_RIGHT:    player1->setVelocityX(player1->getCurrentVel());
                                break;
            case SDLK_SPACE:    player1->setBomb();
                                break;
        }
    else if(event.type==SDL_KEYUP && event.key.repeat==0)
        switch(event.key.keysym.sym){
            // if DOWN KEY and UP KEY is pressing together, and we left DOWN KEY the velY will be set by 0
            case SDLK_UP:       if(player1->getDirectState()!=DOWN)
                                    player1->setVelocityY(0);
                                break;
            case SDLK_DOWN:     if(player1->getDirectState()!=UP)
                                    player1->setVelocityY(0);
                                break;
            case SDLK_LEFT:     if(player1->getDirectState()!=RIGHT)
                                    player1->setVelocityX(0);
                                break;
            case SDLK_RIGHT:    if(player1->getDirectState()!=LEFT)
                                    player1->setVelocityX(0);
                                break;
        }
}

void gameControl2P(SDL_Event& event,character* player1,character* player2){
    if(event.type==SDL_KEYDOWN && event.key.repeat==0)
        switch(event.key.keysym.sym){
            case SDLK_UP:       player1->setVelocityY(-player1->getCurrentVel());
                                break;
            case SDLK_DOWN:     player1->setVelocityY(player1->getCurrentVel());
                                break;
            case SDLK_LEFT:     player1->setVelocityX(-player1->getCurrentVel());
                                break;
            case SDLK_RIGHT:    player1->setVelocityX(player1->getCurrentVel());
                                break;
            case SDLK_RCTRL:    player1->setBomb();
                                break;
            case SDLK_w:        player2->setVelocityY(-player2->getCurrentVel());
                                break;
            case SDLK_s:        player2->setVelocityY(player2->getCurrentVel());
                                break;
            case SDLK_a:        player2->setVelocityX(-player2->getCurrentVel());
                                break;
            case SDLK_d:        player2->setVelocityX(player2->getCurrentVel());
                                break;
            case SDLK_SPACE:      player2->setBomb();
                                  break;
        }
    else if(event.type==SDL_KEYUP && event.key.repeat==0)
        switch(event.key.keysym.sym){
            // if DOWN KEY and UP KEY is pressing together, and we left DOWN KEY the velY will be set by 0
            case SDLK_UP:       if(player1->getDirectState()!=DOWN)
                                    player1->setVelocityY(0);
                                break;
            case SDLK_DOWN:     if(player1->getDirectState()!=UP)
                                    player1->setVelocityY(0);
                                break;
            case SDLK_LEFT:     if(player1->getDirectState()!=RIGHT)
                                    player1->setVelocityX(0);
                                break;
            case SDLK_RIGHT:    if(player1->getDirectState()!=LEFT)
                                    player1->setVelocityX(0);
                                break;
            case SDLK_w:       if(player2->getDirectState()!=DOWN)
                                    player2->setVelocityY(0);
                                break;
            case SDLK_s:     if(player2->getDirectState()!=UP)
                                    player2->setVelocityY(0);
                                break;
            case SDLK_a:     if(player2->getDirectState()!=RIGHT)
                                    player2->setVelocityX(0);
                                break;
            case SDLK_d:    if(player2->getDirectState()!=LEFT)
                                    player2->setVelocityX(0);
                                break;
        }
}





