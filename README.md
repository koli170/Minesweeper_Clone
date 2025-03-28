# Minesweeper Clone

WARNING, This uses a mac port of the olc Pixel game engine library, because of this i do not know if it runs on other machines. The make file and compile instructions have been made with mac in mind.

This is a Minesweeper clone made in c++ using the olc Pixel game engine for a school project. If you've never played minesweeper or just need a refresher on the rules, you can find a guide here:
https://minesweeper.online/


Below you will find an instillation guide and description of its functionality.

## Quick Start
In order to run this program you'll need a mac thats on at least macOS 10.15, homebrew to install the dependencies and a c++ interpereter (clang++ is used in the make file).

1. Clone the repo:
    ```bash
    git clone https://github.com/koli170/Minesweeper_Clone.git
    ```
2. Install dependencies:
    ```bash
    brew install libpng
    ```
3. Compile the program:
    ```bash
    make
    ```
4. The program can be run with:
    ```bash
   ./bin/MineSweeper
    ```

## Functionality

### Controls
The game is mainly controlled with the mouse. You can click on tiles to reveal them, double click on revealed tiles to reveal all tiles around the clicked tile and right click on unrevealed tiles to flag them.

You can also press '1', '2', and '3' to choose a difficulty, 'r' to restart, and 'h' to get a hint.

### Difficulty
When you first open the program you will be greeted with a large minesweeper board, this is the hardest of three difficulties. The difficulty can be changed by pressing the flower button at the top of the board or by pressing '1', '2', or '3' for easy, medium, or hard respectively.

### Playing
To start a game, simply click on a tile on the board. When you do, a board will be generated ensuring that your first click reveals a safe area. Then you're free to click away.

### Restarting
To restart you can press the smiley button at the top of the board, press 'r', or change the difficulty.

### Hints
This version of minesweeper features a hint system! You can get hints by pressing the question mark button at the top of the board or by pressing the 'h' key. When you call for a hint the game will either flag a bomb or reveal a safe area. It prioritizes flagging bombs or revealing areas that you could have gotten with the information you have. It will do this by following the B1 and B2 patterns<sup>1</sup>. In the off chance that it finds nothing with the B1 or B2 patterns<sup>1</sup>, it will flag the bomb with the most revealed tiles around it. This system will help ensure that you can always beat a board, even if you have little experience with the game!

### High scores
When you beat a game, it will list out the time it took you, how many mines you found, and how many hints you used. Then below the game stats you will find you're best times. The times are ranked first by how many hints you used, (the less the better), and then by the time you took. Meaning that a time of 100 seconds with 2 hints will always be better than a time of 30 seconds with 10 hints. Each difficulty has its own high score.


1. (https://minesweeper.online/help/patterns)