# Tic-Tac-Toe Game in C

A simple terminal-based Tic-Tac-Toe game where two players can play together. The game is designed to be played in a terminal.

## Features

- Two-player gameplay
- Real-time input handling
- Visual representation of the game board
- Color-coded turns (Green for active player, Red for inactive player)
- Simple and intuitive controls

![](https://github.com/Abishevs/TicTacToa/blob/master/gifs/tictactoa_1.gif)


## Gameplay

- **Player 1 (P1)** uses the `WASD` keys to move and the `G` key to place their symbol (`X`).
- **Player 2 (P2)** uses the arrow keys to move and the `.` (period) key to place their symbol (`O`).

## Controls

- **P1 Controls:**
  - `w` - Move Up
  - `a` - Move Left
  - `s` - Move Down
  - `d` - Move Right
  - `g` - Place Symbol

- **P2 Controls:**
  - `↑` - Move Up
  - `←` - Move Left
  - `↓` - Move Down
  - `→` - Move Right
  - `.` - Place Symbol

- **General:**
  - `Q` - Quit the game

## Installation and Usage 

  1. **Clone it**:
  ```sh
  git clone https://github.com/Abishevs/TicTacToa.git
  cd TicTacToa/ 
  ```

  2. Compile And Run
  ```sh
  make
  ./build/tictactoa
  ```

### Install
  Into `~/.local/bin`

  ```sh
  make install
  tictactoa
  ```

### Uninstall

  ```sh
  make uninstall
  ```

## Author
Designed by Eduards Abisevs
