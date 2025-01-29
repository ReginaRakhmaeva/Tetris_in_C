#ifndef BACKEND_H
#define BACKEND_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HIGH_SCORE_FILE "highscore.txt"
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

// Функции
GameInfo_t *getGameInfo();
Piece *getCurrentPiece();
//  Функции обработки логики игры
void spawnNewPiece(Piece **piece);
bool canMoveDown(Piece *piece, int **field);
bool canMoveLeft(Piece *piece, int **field);
bool canMoveRight(Piece *piece, int **field);
bool isSpaceAvailableForFullFix(int **field, Piece *piece);
void fixPiece(int **field, Piece *piece);
void fixPartialPiece(int **field, Piece *piece);
int clearFullLines(int **field);
bool handleStartScreenInput();

void rotateMatrix90(int src[4][4], int dest[4][4]);
bool isRotationValid(Piece *piece, int rotated[4][4], int offsetX, int offsetY,
                     int **field);
void applyRotation(Piece *piece, int rotated[4][4], int offsetX, int offsetY);
void rotatePiece(Piece *piece, int **field);
bool isSquarePiece(Piece *piece);
void updateScoreAndLevel(GameInfo_t *game, int linesCleared);
bool updatePiecePosition(Piece *currentPiece, GameInfo_t *game,
                         clock_t *lastTick);
void initializeGame(GameInfo_t *game);
void initializeNext(GameInfo_t *game, const int shapes[7][4][4]);
void createCurrentPiece(Piece **piece, GameInfo_t *game);
void updateNext(GameInfo_t *game, const int shapes[7][4][4]);

#endif  // BACKEND_H
