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

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>  // For usleep()
#include <termios.h> // For terminal control
#include <fcntl.h>   // For non-blocking input
                     
#include "colors.h"

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
void set_raw_mode() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

// Function to reset terminal to normal mode
// NO IDEA HOW THIS WORKS its a copy pastaa
void reset_raw_mode() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

// Function to check if a key has been pressed
// NO IDEA HOW THIS WORKS its a copy pastaa
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void clearScreen(){
        printf("\033[2J");
}

void printFrame(char *frame[ROWS][COLS]) {
/*
P1        P2 // Score
┌───┬───┬───┐ 
│   │   │   │
├───┼───┼───┤  
│   │   │   │
├───┼───┼───┤
│   │   │   │
└───┴───┴───┘
*/
    for (int x = 0; x < ROWS; x++) {
        for (int y = 0; y < COLS; y++) {
            printf("%s", frame[x][y]);
        }
        printf("\n");
    }
}

// Invert active state
void toggleActive(Player *p1, Player *p2){
    p1->active = !p1->active;
    p2->active = !p2->active;

}

Player *getActivePlayer(Game *game){
    if (game->p1->active){
        return game->p1;
    }
    return game->p2;
}

void placeSymbol(Game *game) {
    if (game->y >= 1 && game->y <= 5 && game->x >= 1 && game->x <= 11) {

        if(strcmp(game->frame[game->y][game->x], " ") == 0){ 
            //Only place symbol if cell is empty str

            switch (getActivePlayer(game)->symbol) {
                case X: game->frame[game->y][game->x] = "X"; break;
                case O: game->frame[game->y][game->x] = "O"; break;
            }

            toggleActive(game->p1, game->p2);
        }

    } else {
        fprintf(stderr, "Error: Coordinates out of bounds\n");
    }



}

void initializeFrame(char *frame[ROWS][COLS]) {
    const char *initialFrame[ROWS][COLS] = {
        {TOP_LEFT_CORNER, HORIZONTAL, HORIZONTAL, HORIZONTAL, T_JUNCTION, HORIZONTAL, HORIZONTAL, HORIZONTAL, T_JUNCTION, HORIZONTAL, HORIZONTAL, HORIZONTAL, TOP_RIGHT_CORNER},
        {VERTICAL, " ", " ", " ", VERTICAL, " ", " ", " ", VERTICAL, " ", " ", " ", VERTICAL},
        {LEFT_T, HORIZONTAL, HORIZONTAL, HORIZONTAL, CROSS, HORIZONTAL, HORIZONTAL, HORIZONTAL, CROSS, HORIZONTAL, HORIZONTAL, HORIZONTAL, RIGHT_T},
        {VERTICAL, " ", " ", " ", VERTICAL, " ", " ", " ", VERTICAL, " ", " ", " ", VERTICAL},
        {LEFT_T, HORIZONTAL, HORIZONTAL, HORIZONTAL, CROSS, HORIZONTAL, HORIZONTAL, HORIZONTAL, CROSS, HORIZONTAL, HORIZONTAL, HORIZONTAL, RIGHT_T},
        {VERTICAL, " ", " ", " ", VERTICAL, " ", " ", " ", VERTICAL, " ", " ", " ", VERTICAL},
        {BOTTOM_LEFT_CORNER, HORIZONTAL, HORIZONTAL, HORIZONTAL, BOTTOM_T, HORIZONTAL, HORIZONTAL, HORIZONTAL, BOTTOM_T, HORIZONTAL, HORIZONTAL, HORIZONTAL, BOTTOM_RIGHT_CORNER}
    };

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            frame[i][j] = (char *)initialFrame[i][j];
        }
    }
}

void printScore(Player *active_player){
    // Amount of padding = COLS - (2 + 2) chars
    int padding = COLS - 4;
    
    if (active_player->symbol == X){
        printf(GREEN "P1" RESET);
    } else {
        printf(RED "P1" RESET);
    }

    for (int i = 0; i < padding;i++){
        printf(" ");
    }
    if (active_player->symbol == O){
        printf(GREEN "P2" RESET);
    } else {
        printf(RED "P2" RESET);
    }
}


void moveCursor(Game *game, Direction direction){
    switch (direction) {
        case UP: if (game->y > 1) game->y -= 2; break;
        case DOWN: if (game->y < 5) game->y += 2; break;
        case LEFT: if (game->x > 2) game->x -= 4; break;
        case RIGHT: if (game->x < 10) game->x += 4; break;
    }
}

void handleMovement(Game *game, char direction) {
    if (game->p1->active){
    switch (direction) {
        case 'w': moveCursor(game, UP); break; 
        case 's': moveCursor(game, DOWN); break;
        case 'a':  moveCursor(game, LEFT); break;
        case 'd':  moveCursor(game, RIGHT); break;
    }
    } else {
    switch (direction) {
        // Arrow keys
        case 'A': moveCursor(game, UP); break; 
        case 'B': moveCursor(game, DOWN); break;
        case 'D': moveCursor(game, LEFT); break;
        case 'C': moveCursor(game, RIGHT); break;
    }
    }
}

int checkWinner(Game *game) {
    // Check rows, columns, and diagonals for a win
    // Return 1 if X wins, 2 if O wins, 0 otherwise
    // This is just a placeholder; you'll need to implement the actual logic
    return 0;
}

void resetGame(Game *game) {
    initializeFrame(game->frame);
    game->x = 2;
    game->y = 1;
    game->p1->active = 1;
    game->p2->active = 0;
}

void run(Game *game) {
    set_raw_mode();

    for (;;) {
        // Clear the screen
        clearScreen();

        printf("\033[1;1H"); // Move cursor to top-left corner
        printScore(getActivePlayer(game));
        printf("\033[2;1H"); // Move cursor below the score
        printFrame(game->frame);
        printf("X: %d, Y: %d", game->x, game->y); 

        // Move the cursor to first cell
        // Account for Score and Frame elements
        printf("\033[%d;%dH", game->y + 2, game->x + 1);

        // Flush the output to the terminal
        fflush(stdout);

        // Check for key press
        if (kbhit()) {
            char ch = getchar();
            if (ch == 'Q') break;  // Quit on 'Q'
            if (ch == 'g' || ch == '.') {
                placeSymbol(game);
                if (checkWinner(game) != 0) {
                    printf("\nPlayer wins!\n");
                    break;
                }
            }
            handleMovement(game, ch);
        }

        usleep(1000);  // Sleep for 1ms
    }

    // Clean Up
    printf("\n");
    clearScreen();

    reset_raw_mode();
}

int main() {
    char *frame[ROWS][COLS];
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
