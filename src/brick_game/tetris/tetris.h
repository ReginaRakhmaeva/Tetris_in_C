#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdbool.h>

// Константы
#define ROWS 20
#define COLS 10
#define SUCCESS 0
#define WIN_INIT(size) initNcurses()
#define GET_USER_INPUT getch()

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
void rotatePiece(Piece *piece, int **field);
GameInfo_t *getGameInfo();
Piece *getCurrentPiece();
GameInfo_t updateCurrentState();
void initNcurses();
void cleanupNcurses(GameInfo_t *game);

#endif  // TETRIS_H
