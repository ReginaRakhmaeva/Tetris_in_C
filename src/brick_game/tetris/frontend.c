#include "frontend.h"

#include <ncurses.h>

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

bool handleStartScreenInput() {
  while (true) {
    int ch = GET_USER_INPUT;
    if (ch == '\n') {        // Нажата клавиша Enter
      return true;           // Начать игру
    } else if (ch == 'q') {  // Нажата клавиша q
      return false;          // Выйти из игры
    }
  }
}

// PERERABOTAT' s BACKEND
void showGameOverScreen(GameInfo_t *game) {
  clear();  // Очистка экрана
  mvprintw(ROWS / 2 - 1, COLS / 2 - 5, "GAME OVER");
  mvprintw(ROWS / 2, COLS / 2 - 8, "Your Score: %d", game->score);
  mvprintw(ROWS / 2 + 1, COLS / 2 - 10, "Press ENTER to Restart");
  mvprintw(ROWS / 2 + 2, COLS / 2 - 7, "or q to Quit");

  refresh();  // Обновляем экран
  bool fl = true;
  while (fl) {
    int ch = GET_USER_INPUT;
    if (ch == 'q') {  // Quit
      cleanupNcurses(game);
      fl = false;
    }
  }
}