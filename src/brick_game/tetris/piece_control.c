/**
 * @file piece_control.c
 * @brief Управление фигурами в игре Тетрис.
 *
 * Этот файл содержит функции для создания, управления, проверки и вращения
 * фигур в игровом поле Тетрис. Он включает логику генерации новых фигур,
 * проверки их движения и размещения на игровом поле.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game_logic.h"

/**
 * @brief Инициализирует следующую фигуру (`next`), если она не задана.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую игровую информацию.
 * @param shapes Массив фигур, представленных в виде матриц.
 */
void initializeNext(GameInfo_t *game, const int shapes[7][4][4]) {
  if (!game->next) {
    game->next = (int **)malloc(sizeof(int *) * 4);
    for (int i = 0; i < 4; i++) {
      game->next[i] = (int *)calloc(4, sizeof(int));
    }
    int shapeIndex = rand() % 7;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        game->next[i][j] = shapes[shapeIndex][i][j];
      }
    }
  }
}

/**
 * @brief Создаёт текущую фигуру из следующей (`next`).
 *
 * @param piece Указатель на указатель структуры Piece, представляющей фигуру.
 * @param game Указатель на структуру GameInfo_t, содержащую игровую информацию.
 */
void createCurrentPiece(Piece **piece, GameInfo_t *game) {
  *piece = (Piece *)malloc(sizeof(Piece));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*piece)->shape[i][j] = game->next[i][j];
    }
  }
  (*piece)->x = COLS / 2 - 2;
  (*piece)->y = 0;
}

/**
 * @brief Обновляет следующую фигуру (`next`) новой случайной фигурой.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую игровую информацию.
 * @param shapes Массив фигур, представленных в виде матриц.
 */
void updateNext(GameInfo_t *game, const int shapes[7][4][4]) {
  int shapeIndex = rand() % 7;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next[i][j] = shapes[shapeIndex][i][j];
    }
  }
}

/**
 * @brief Основная функция для создания новой фигуры.
 *
 * @param piece Указатель на указатель структуры Piece, представляющей фигуру.
 */
void spawnNewPiece(Piece **piece) {
  static const int shapes[7][4][4] = {
      {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Линия
      {{1, 1}, {1, 1}, {0, 0}, {0, 0}},              // Квадрат
      {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}, {0, 0, 0}},  // Z-образная
      {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}, {0, 0, 0}},  // Обратная Z
      {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}},  // T-образная
      {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}},  // L-образная
      {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}}   // Обратная L
  };

  GameInfo_t *game = getGameInfo();

  initializeNext(game, shapes);
  createCurrentPiece(piece, game);
  updateNext(game, shapes);
}

/**
 * @brief Проверяет, может ли фигура двигаться влево.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param field Двумерный массив игрового поля.
 * @return true, если движение влево возможно, иначе false.
 */
bool canMoveLeft(Piece *piece, int **field) {
  bool canMove = true;

  if (piece == NULL) {
    canMove = false;
  }
  for (int i = 0; i < 4 && canMove; i++) {
    for (int j = 0; j < 4 && canMove; j++) {
      if (piece->shape[i][j]) {
        if (piece->x + j - 1 < 0 || field[piece->y + i][piece->x + j - 1]) {
          canMove = false;
        }
      }
    }
  }
  return canMove;
}

/**
 * @brief Проверяет, может ли фигура двигаться вправо.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param field Двумерный массив игрового поля.
 * @return true, если движение вправо возможно, иначе false.
 */
bool canMoveRight(Piece *piece, int **field) {
  bool canMove = true;

  if (piece == NULL) {
    canMove = false;
  }
  for (int i = 0; i < 4 && canMove; i++) {
    for (int j = 0; j < 4 && canMove; j++) {
      if (piece->shape[i][j]) {
        if (piece->x + j + 1 >= COLS || field[piece->y + i][piece->x + j + 1]) {
          canMove = false;
        }
      }
    }
  }

  return canMove;
}

/**
 * @brief Проверяет, может ли фигура двигаться вниз.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param field Двумерный массив игрового поля.
 * @return true, если движение вниз возможно, иначе false.
 */
bool canMoveDown(Piece *piece, int **field) {
  bool canMove = true;

  if (piece == NULL) {
    canMove = false;
  }
  for (int i = 0; i < 4 && canMove; i++) {
    for (int j = 0; j < 4 && canMove; j++) {
      if (piece->shape[i][j]) {
        if (piece->y + i + 1 >= ROWS || field[piece->y + i + 1][piece->x + j]) {
          canMove = false;
        }
      }
    }
  }
  return canMove;
}

/**
 * @brief Фиксирует фигуру на игровом поле.
 *
 * @param field Двумерный массив игрового поля.
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 */
void fixPiece(int **field, Piece *piece) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        int newX = piece->x + j;
        int newY = piece->y + i;
        field[newY][newX] = 1;
      }
    }
  }
  free(piece);
}

/**
 * @brief Проверяет, является ли фигура квадратной.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @return true, если фигура квадратная, иначе false.
 */
bool isSquarePiece(Piece *piece) {
  return piece->shape[1][1] == 1 && piece->shape[0][0] == 1 &&
         piece->shape[0][1] == 1 && piece->shape[1][0] == 1;
}

/**
 * @brief Вращает матрицу на 90 градусов по часовой стрелке.
 *
 * @param src Исходная матрица.
 * @param dest Результирующая матрица.
 */
void rotateMatrix90(int src[4][4], int dest[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest[j][3 - i] = src[i][j];
    }
  }
}

/**
 * @brief Проверяет, является ли вращение фигуры допустимым.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param rotated Вращённая матрица фигуры.
 * @param offsetX Смещение по оси X.
 * @param offsetY Смещение по оси Y.
 * @param field Двумерный массив игрового поля.
 * @return true, если вращение допустимо, иначе false.
 */
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

/**
 * @brief Вращает фигуру, если это возможно.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param field Двумерный массив игрового поля.
 */
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

/**
 * @brief Применяет вращение к фигуре.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param rotated Вращённая матрица фигуры.
 * @param offsetX Смещение по оси X.
 * @param offsetY Смещение по оси Y.
 */
void applyRotation(Piece *piece, int rotated[4][4], int offsetX, int offsetY) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      piece->shape[i][j] = rotated[i][j];
    }
  }
  piece->x += offsetX;
  piece->y += offsetY;
}

/**
 * @brief Фиксирует только часть фигуры на игровом поле. Используется когда
 * падающая фигура может закрепиться к полю только нижней частью, то есть под
 * ней свобона только одна строка.
 *
 * @param field Двумерный массив игрового поля.
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 */
void fixPartialPiece(int **field, Piece *piece) {
  bool hasBlock = false;
  for (int i = 3; i >= 0 && !hasBlock; i--) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        hasBlock = true;
        int newX = piece->x + j;
        if (newX >= 0 && newX < COLS) {
          field[0][newX] = 1;
        }
      }
    }
  }
  free(piece);
}