#ifndef GAME_INFO_H
#define GAME_INFO_H
#include <stdio.h>

// Структуры
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  int x;
  int y;
  int shape[4][4];
} Piece;

GameInfo_t *getGameInfo();

Piece *getCurrentPiece();
#include "piece_create.h"

#endif  // GAME_INFO_H