#include "colors.h"
#include "tictactoa.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>  // For usleep()
#include <termios.h> // For terminal control
#include <fcntl.h>   // For non-blocking input
                     

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
    // Check rows
    for (int i = 1; i <= 5; i += 2) {
        if (strcmp(game->frame[i][2], game->frame[i][6]) == 0 &&
            strcmp(game->frame[i][6], game->frame[i][10]) == 0 &&
            strcmp(game->frame[i][2], " ") != 0) {
            return game->frame[i][2][0] == 'X' ? 1 : 2;
        }
    }

    // Check columns
    for (int j = 2; j <= 10; j += 4) {
        if (strcmp(game->frame[1][j], game->frame[3][j]) == 0 &&
            strcmp(game->frame[3][j], game->frame[5][j]) == 0 &&
            strcmp(game->frame[1][j], " ") != 0) {
            return game->frame[1][j][0] == 'X' ? 1 : 2;
        }
    }

    // Check diagonals
    if (strcmp(game->frame[1][2], game->frame[3][6]) == 0 &&
        strcmp(game->frame[3][6], game->frame[5][10]) == 0 &&
        strcmp(game->frame[1][2], " ") != 0) {
        return game->frame[1][2][0] == 'X' ? 1 : 2;
    }

    if (strcmp(game->frame[1][10], game->frame[3][6]) == 0 &&
        strcmp(game->frame[3][6], game->frame[5][2]) == 0 &&
        strcmp(game->frame[1][10], " ") != 0) {
        return game->frame[1][10][0] == 'X' ? 1 : 2;
    }

    // Check for draw
    for (int i = 1; i <= 5; i += 2) {
        for (int j = 2; j <= 10; j += 4) {
            if (strcmp(game->frame[i][j], " ") == 0) {
                return 0; // Still empty cells, no draw
            }
        }
    }

    return -1; // Draw
}

void resetGame(Game *game) {
    initializeFrame(game->frame);
    game->x = 2;
    game->y = 1;
    game->p1->active = 1;
    game->p2->active = 0;
}

void gameOverScreen(Game *game, int winner) {
    clearScreen();

    if (winner > 0){
        printf(GREEN "P%d " RESET " wins!\n", winner);
    } else {
        printf(CYAN "It's a draw!\n" RESET);
    }
    printf(GREEN "Press any key to restart...\n" RESET);
    fflush(stdout);

    // Wait for user input
    getchar();

    resetGame(game);
}

void ReadyPlayerOne() {
    clearScreen();
    
    printf(CYAN "=============================\n" RESET);
    printf(CYAN "        TIC-TAC-TOE\n" RESET);
    printf(CYAN "=============================\n" RESET);
    
    printf(GREEN "Rules:\n" RESET);
    printf("1. Player 1 (P1) always plays as 'X'.\n");
    printf("2. Player 2 (P2) always plays as 'O'.\n");
    printf("3. Take turns to place your symbol on the board.\n");
    printf("4. The first player to get three of their symbols in a row\n (horizontally, vertically, or diagonally) wins.\n");
    printf("5. If all cells are filled and no player has three in a row, \nthe game is a draw.\n\n");

    printf(GREEN "Keybindings:\n" RESET);
    printf("  P1 Controls:\t\t\tP2 Controls:\n");
    printf("  w - Move Up\t\t\t↑ - Move Up\n");
    printf("  a - Move Left\t\t\t← - Move Left\n");
    printf("  s - Move Down\t\t\t↓ - Move Down\n");
    printf("  d - Move Right\t\t→ - Move Right\n");
    printf("  g - Place Symbol\t\t. - Place Symbol\n\n");

    printf(CYAN "=============================\n" RESET);
    printf(CYAN "       READY PLAYER ONE?\n" RESET);
    printf(CYAN "=============================\n" RESET);
    printf(GREEN "Press any key to start the game...\n" RESET);
    fflush(stdout);

    // Wait for user input
    getchar();
}

void run(Game *game) {
    set_raw_mode();

    ReadyPlayerOne();

    for (;;) {
        // Clear the screen
        clearScreen();

        printf("\033[1;1H"); // Move cursor to top-left corner
        printScore(getActivePlayer(game));
        printf("\033[2;1H"); // Move cursor below the score
        printFrame(game->frame);
        // printf("X: %d, Y: %d\n", game->x, game->y);  // For debugging pos
        printf("Press " RED"'Q'"RESET " to exit the game");

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
                int winner = checkWinner(game);
                if (winner != 0) {
                    gameOverScreen(game, winner);
                }
            }
            handleMovement(game, ch);
        }

        usleep(10000);  // Sleep for 10ms
    }

    // Clean Up
    printf("\n");
    clearScreen();

    reset_raw_mode();
}
