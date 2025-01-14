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
GameInfo_t *getGameInfo();
Piece *getCurrentPiece();
void spawnNewPiece(Piece **piece);
void game_loop();
bool canMoveDown(Piece *piece, int **field);
void userInput(UserAction_t action, bool hold);
void renderStartScreen();
bool handleStartScreenInput();
GameInfo_t updateCurrentState();
int clearFullLines(int **field);
void fixPiece(int **field, Piece *piece);
void initNcurses();
void cleanupNcurses(GameInfo_t *game);
void fixPartialPiece(int **field, Piece *piece);
bool isSpaceAvailableForFullFix(int **field, Piece *piece);
bool canMoveLeft(Piece *piece, int **field);
bool canMoveRight(Piece *piece, int **field);
bool isSquarePiece(Piece *piece);
void rotateMatrix90(int src[4][4], int dest[4][4]);
bool isRotationValid(Piece *piece, int rotated[4][4], int offsetX, int offsetY,
                     int **field);
void applyRotation(Piece *piece, int rotated[4][4], int offsetX, int offsetY);

#endif  // TETRIS_H
