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

bool isSquarePiece(Piece *piece) {
  return piece->shape[1][1] == 1 && piece->shape[0][0] == 1 &&
         piece->shape[0][1] == 1 && piece->shape[1][0] == 1;
}

void rotateMatrix90(int src[4][4], int dest[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest[j][3 - i] = src[i][j];
    }
  }
}

bool isRotationValid(Piece *piece, int rotated[4][4], int offsetX, int offsetY,
                     int **field) {
  bool isValid = true;

  for (int i = 0; i < 4 && isValid; i++) {
    for (int j = 0; j < 4 && isValid; j++) {
      if (rotated[i][j]) {
        int newX = piece->x + j + offsetX;
        int newY = piece->y + i + offsetY;

        if (newX < 0 || newX >= COLS || newY >= ROWS ||
            (newY >= 0 && field[newY][newX])) {
          isValid = false;
        }
      }
    }
  }
  return isValid;
}

void applyRotation(Piece *piece, int rotated[4][4], int offsetX, int offsetY) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      piece->shape[i][j] = rotated[i][j];
    }
  }
  piece->x += offsetX;
  piece->y += offsetY;
}

void rotatePiece(Piece *piece, int **field) {
  if (isSquarePiece(piece)) {
    return;  // Проверка аргументов
  }

  int rotated[4][4];
  rotateMatrix90(piece->shape, rotated);

  const int offsets[5][2] = {
      {0, 0},   // No offset
      {-1, 0},  // Left
      {1, 0},   // Right
      {0, -1},  // Up
      {0, 1}    // Down
  };

  bool rotationApplied = false;

  for (int k = 0; k < 5 && !rotationApplied; k++) {
    int offsetX = offsets[k][0];
    int offsetY = offsets[k][1];

    if (isRotationValid(piece, rotated, offsetX, offsetY, field)) {
      applyRotation(piece, rotated, offsetX, offsetY);
      rotationApplied = true;
    }
  }

  if (!rotationApplied) {
    if (piece->x + 3 >= COLS && canMoveLeft(piece, field)) {
      piece->x -= 1;
    } else if (piece->x < 0 && canMoveRight(piece, field)) {
      piece->x += 1;
    }
    rotatePiece(piece, field);  // Рекурсия для повторной попытки
  }
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
