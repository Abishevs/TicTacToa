//tictactoa.h
#ifndef TICTACTOA_H
#define TICTACTOA_H

#define TOP_LEFT_CORNER "┌"
#define TOP_RIGHT_CORNER "┐"
#define BOTTOM_LEFT_CORNER "└"
#define BOTTOM_RIGHT_CORNER "┘"
#define HORIZONTAL "─"
#define VERTICAL "│"
#define T_JUNCTION "┬"
#define CROSS "┼"
#define BOTTOM_T "┴"
#define LEFT_T "├"
#define RIGHT_T "┤"

#define ROWS 7
#define COLS 13 

typedef enum Symbol {
    X, // P1 Always plays as X
    O
} Symbol;

typedef struct Player {
    Symbol symbol; // Play with X or O
    int active; // 0 false aka not active, 1 active
    
} Player;


typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct Game {
    char *frame[ROWS][COLS];
    Player *p1;
    Player *p2;
    int x; // Horizontal direction
    int y; // Vertical direction
           
} Game;


// Function to set terminal to raw mode
// NO IDEA HOW THIS WORKS its a copy pastaa
void set_raw_mode();

// Function to reset terminal to normal mode
// NO IDEA HOW THIS WORKS its a copy pastaa
void reset_raw_mode();

// Function to check if a key has been pressed
// NO IDEA HOW THIS WORKS its a copy pastaa
int kbhit();

void clearScreen();

void printFrame(char *frame[ROWS][COLS]);

// Inverts Players active field:
// To toggle turns
void toggleActive(Player *p1, Player *p2);

// Returns ptr to active player:
// Aka whos turn it is to play
Player *getActivePlayer(Game *game);

// Place symbol in frame
void placeSymbol(Game *game); 

// Init frame for game struct
void initializeFrame(char *frame[ROWS][COLS]); 

// Determines which color player get:
// To indicate whos turn it is
void printScore(Player *active_player);

//Moves cursor:
// UP
// DOWN
// LEFT
// RIGHT
void moveCursor(Game *game, Direction direction);

// Parser user KB input:
// Calls moveCursor
void handleMovement(Game *game, char direction); 

// Returns the winner: 
// 1 -> P1 wins, 
// 2 -> P2 wins,
// -1 -> if its a draw,
// 0 no winner yet
int checkWinner(Game *game);

// Reset game struct to initial values
void resetGame(Game *game);

// Reset sceene after winner is found
void gameOverScreen(Game *game, int winner);

// First welcome screen
void ReadyPlayerOne();

void run(Game *game); 

#endif
