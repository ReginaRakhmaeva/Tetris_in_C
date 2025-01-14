#include "tetris.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "backend.h"
#include "frontend.h"
#include "tetris.h"

GameInfo_t *getGameInfo() {
  static GameInfo_t game;
  return &game;
}
Piece *getCurrentPiece() {
  static Piece *piece = NULL;
  if (!piece) {
    spawnNewPiece(&piece);  // Инициализация новой фигуры
  }
  return piece;
}

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(50);
}

void cleanupNcurses(GameInfo_t *game) {
  endwin();
  for (int i = 0; i < ROWS; i++) {
    free(game->field[i]);
  }
  free(game->field);

  for (int i = 0; i < 4; i++) {
    free(game->next[i]);
  }
  free(game->next);
}

void cleanupNcursesstart() {
  endwin();  // Завершение работы с ncurses
}

int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  if (!showStartScreen()) {
    cleanupNcursesstart();
  } else
    game_loop();

  return SUCCESS;
}
