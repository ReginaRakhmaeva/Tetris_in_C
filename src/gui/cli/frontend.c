#include "frontend.h"

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  if (!showStartScreen()) {
    cleanupNcursesstart();
  } else
    game_loop();

  return SUCCESS;
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

int processUserInput() { return GET_USER_INPUT; }
void clearField() {
  clear();
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
}

void drawStaticField(int **field) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (field[i][j]) {
        mvprintw(i + 1, j * 2 + 1, "[]");
      }
    }
  }
}

void drawPiece(Piece *piece) {
  if (!piece) return;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        mvprintw(piece->y + i + 1, (piece->x + j) * 2 + 1, "[]");
      }
    }
  }
}

void drawGameInfo(GameInfo_t *game) {
  mvprintw(2, COLS * 2 + 5, "Score: %d", game->score);
  mvprintw(3, COLS * 2 + 5, "Level: %d", game->level);
  mvprintw(4, COLS * 2 + 5, "High Score: %d", game->high_score);
  mvprintw(6, COLS * 2 + 5, "Next:");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->next[i][j]) {
        mvprintw(6 + i, COLS * 2 + 11 + j * 2, "[]");
      }
    }
  }
}

void renderStartScreen() {
  clear();

  mvprintw(ROWS / 2 - 5, COLS / 2, "TETRIS GAME");
  mvprintw(ROWS / 2, COLS / 2 - 4, "Press ENTER to Start");
  mvprintw(ROWS / 2 + 1, COLS / 2, "or q to Quit");
  mvprintw(ROWS / 2 + 4, COLS / 2 + 2, "RULES");
  mvprintw(ROWS / 2 + 5, COLS / 2 - 3, "Arrow Keys: Move");
  mvprintw(ROWS / 2 + 6, COLS / 2 - 2, "Space: Rotate");
  mvprintw(ROWS / 2 + 7, COLS / 2 - 3, "p: Pause, q: Quit");

  refresh();
}

bool showStartScreen() {
  renderStartScreen();              // Отобразить экран
  return handleStartScreenInput();  // Обработать ввод
}

void drawField(GameInfo_t *game) {
  clearField();
  drawStaticField(game->field);
  drawPiece(getCurrentPiece());
  drawGameInfo(game);
  refresh();
}

// PERERABOTAT' s BACKEND
void displayGameOverScreen(GameInfo_t *game) {
  clear();
  mvprintw(ROWS / 2 - 1, COLS / 2 - 5, "GAME OVER");
  mvprintw(ROWS / 2, COLS / 2 - 8, "Your Score: %d", game->score);
  mvprintw(ROWS / 2 + 1, COLS / 2 - 10, "Press ENTER to Restart");
  mvprintw(ROWS / 2 + 2, COLS / 2 - 7, "or q to Quit");

  refresh();
}

bool handleGameOverInput() {
  int ch = processUserInput();
  if (ch == 'q') {
    return false;
  }
  return true;
}

void showGameOverScreen(GameInfo_t *game) {
  displayGameOverScreen(game);

  bool continueGame = true;
  while (continueGame) {
    continueGame = handleGameOverInput();
  }

  cleanupNcurses(game);
}
