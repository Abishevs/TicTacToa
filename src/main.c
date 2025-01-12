/*
P1        P2
┌───┬───┬───┐ 
│   │   │   │
├───┼───┼───┤  
│ X │ O │   │
├───┼───┼───┤
│   │   │   │
└───┴───┴───┘

A simple Tic-Tac-Toa
Where to players can play toghether. 


Turns are displayed with color
eg Green = active (Players turn)
Red = inactive

Gameplay:
P1 has wasd keys
And g key 
P2 uses arrow keys
and places move with . (period) symbol

DESIGNED BY EDUARDS ABISEVS
*/

#include "tictactoa.h"

int main() {
    Player p1 = {X, 1};
    Player p2 = {O, 0};
    Game game;

    initializeFrame(game.frame);
    game.p1 = &p1;
    game.p2 = &p2;
    game.x = 2;
    game.y = 1;

    run(&game);

    return 0;
}
