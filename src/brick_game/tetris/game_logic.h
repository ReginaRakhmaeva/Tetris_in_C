#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game_info.h"

#define HIGH_SCORE_FILE "brick_game/tetris/highscore.txt"
#define ROWS 20
#define COLS 10
// Константы
#define SUCCESS 0

// Перечисления
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

int loadHighScore();
void saveHighScore(int highScore);
bool updatePiecePosition(Piece *currentPiece, GameInfo_t *game,
                         clock_t *lastTick);
void initializeGame(GameInfo_t *game);
void updateScore(GameInfo_t *game, int linesCleared);
void updateLevelAndSpeed(GameInfo_t *game);
int clearFullLines(int **field);
#include "piece_create.h"
#include "piece_fix.h"
#include "piece_move.h"
#endif  // GAME_LOGIC_H
