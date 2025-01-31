/**
 * @file piece_fix.c
 * @brief Функции для фиксации фигур на игровом поле в Тетрисе.
 *
 * Этот файл содержит функции для закрепления фигур на поле,
 * проверки возможности полной фиксации и обработки частичной фиксации.
 */

#include "../header/piece_fix.h"

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
/**
 * @brief Проверяет, доступно ли место для полной фиксации фигуры на поле.
 *
 * @param field Двумерный массив, представляющий игровое поле.
 * @param piece Указатель на фигуру.
 *
 * @return true, если место доступно; false, если место недоступно.
 */
bool isSpaceAvailableForFullFix(int **field, Piece *piece) {
  bool isAvailable = true;

  for (int i = 0; i < 4 && isAvailable; i++) {
    for (int j = 0; j < 4 && isAvailable; j++) {
      if (piece->shape[i][j]) {
        int newX = piece->x + j;
        int newY = piece->y + i;
        if (field[newY][newX]) {
          isAvailable = false;
        }
      }
    }
  }
  return isAvailable;
}
