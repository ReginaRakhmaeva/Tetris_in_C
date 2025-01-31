/**
 * @file piece_move.c
 * @brief Функции для перемещения и вращения фигур в Тетрисе.
 *
 * Этот файл содержит функции для проверки возможности движения фигур влево,
 * вправо и вниз, а также для их вращения и проверки корректности вращения.
 */

#include "../header/piece_move.h"

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
 * @brief Пытается повернуть фигуру на 90 градусов по часовой стрелке.
 *
 * Функция проверяет возможность поворота фигуры. Если фигура упирается в стену
 * или другие блоки, пробуются различные смещения (offset) для успешного
 * выполнения поворота. Если фигура квадратная, поворот не выполняется.
 *
 * @param piece Указатель на структуру `Piece`, представляющую текущую фигуру.
 * @param field Двумерный массив `int`, представляющий игровое поле.
 */
void rotatePiece(Piece *piece, int **field) {
  if (isSquarePiece(piece)) {
  } else {
    int rotated[4][4];
    rotateMatrix90(piece->shape, rotated);

    const int offsets[5][2] = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    bool rotationApplied = false;

    for (int k = 0; k < 5 && !rotationApplied; k++) {
      int offsetX = offsets[k][0];
      int offsetY = offsets[k][1];

      if (isRotationValid(piece, rotated, offsetX, offsetY, field)) {
        applyRotation(piece, rotated, offsetX, offsetY);
        rotationApplied = true;
      }
    }
  }
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
