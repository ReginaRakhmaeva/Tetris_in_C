#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 20
#define COLS 10

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

GameInfo_t *getGameInfo() {
  static GameInfo_t game;
  return &game;
}

Piece *getCurrentPiece() {
  static Piece *piece = NULL;
  return piece;
}
void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

void drawField(GameInfo_t *game);
void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(500);
}

void drawField(GameInfo_t *game) {
  clear();
  // Отрисовка рамки
  for (int i = 0; i <= ROWS; i++) {
    mvprintw(i, 0, "|");
    mvprintw(i, COLS * 2 + 1, "|");
  }
  for (int j = 0; j <= COLS * 2; j++) {
    mvprintw(0, j, "-");
    mvprintw(ROWS + 1, j, "-");
  }
  mvprintw(0, 0, "+");
  mvprintw(0, COLS * 2 + 1, "+");
  mvprintw(ROWS + 1, 0, "+");
  mvprintw(ROWS + 1, COLS * 2 + 1, "+");

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (game->field[i][j]) {
        mvprintw(i + 1, j * 2 + 1, "[]");
      } else {
        mvprintw(i + 1, j * 2 + 1, "  ");
      }
    }
  }

  // Отрисовка текущей фигуры
  // !!!дописать

  // Вывод игровой информации
  mvprintw(2, COLS * 2 + 5, "Score: %d", game->score);
  mvprintw(3, COLS * 2 + 5, "Level: %d", game->level);
  mvprintw(4, COLS * 2 + 5, "High Score: %d", game->high_score);

  refresh();  // Обновляем экран
}

void start() {
  GameInfo_t *game = getGameInfo();
  game->score = 0;
  game->high_score = 0;
  game->level = 1;
  game->speed = 100;
  game->pause = 0;

  game->field = (int **)malloc(sizeof(int *) * ROWS);
  for (int i = 0; i < ROWS; i++) {
    game->field[i] = (int *)malloc(sizeof(int) * COLS);
    for (int j = 0; j < COLS; j++) {
      game->field[i][j] = 0;
    }
  }
}
void cleanupNcurses() { endwin(); }
int main() {
  GameInfo_t *game = getGameInfo();
  //  Piece *currentPiece = getCurrentPiece();
  initNcurses();
  start();
  drawField(game);
  cleanupNcurses();
  return 0;
}