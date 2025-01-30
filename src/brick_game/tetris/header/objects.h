/**
 * @file objects.h
 * @brief Определение структур данных, используемых в игре Тетрис.
 */

#ifndef OBJECTS_H
#define OBJECTS_H

#include <ncurses.h>

#include "defines.h"

/**
 * @struct GameInfo_t
 * @brief Структура, содержащая информацию о состоянии игры.
 */
typedef struct {
  int **field; /**< Двумерный массив, представляющий игровое поле */
  int **next;  /**< Следующая фигура */
  int score;   /**< Текущий счет */
  int high_score; /**< Рекордный счет */
  int level;      /**< Текущий уровень */
  int speed;      /**< Скорость падения фигур */
  int pause;      /**< Флаг паузы */
} GameInfo_t;

/**
 * @struct Piece
 * @brief Структура, представляющая фигуру Тетриса.
 */
typedef struct {
  int x; /**< Координата X фигуры на поле */
  int y; /**< Координата Y фигуры на поле */
  int shape[4][4]; /**< Матрица 4x4, представляющая форму фигуры */
} Piece;

#endif  // OBJECTS_H
