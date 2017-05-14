#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <character.h>

void gameControl1P(SDL_Event& event,character* player1);
void gameControl2P(SDL_Event& event,character* player1,character* player2);

#endif // GAMECONTROLLER_H
