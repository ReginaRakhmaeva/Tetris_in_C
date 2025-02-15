/**
 * @file piece_move.h
 * @brief Функции для перемещения и поворота фигур в игре.
 */

#ifndef PIECE_MOVE_H
#define PIECE_MOVE_H

#include "game_logic.h"

/**
 * @brief Проверяет, может ли фигура двигаться вниз.
 *
 * Функция проверяет, есть ли пространство под фигурой, чтобы она могла
 * продолжить движение вниз.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param field Двумерный массив игрового поля.
 *
 * @return true, если фигура может двигаться вниз; false, если она не может.
 */
bool canMoveDown(Piece *piece, int **field);

/**
 * @brief Проверяет, может ли фигура двигаться влево.
 *
 * Функция проверяет, есть ли пространство слева от фигуры, чтобы она могла
 * продолжить движение влево.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param field Двумерный массив игрового поля.
 *
 * @return true, если фигура может двигаться влево; false, если она не может.
 */
bool canMoveLeft(Piece *piece, int **field);

/**
 * @brief Проверяет, может ли фигура двигаться вправо.
 *
 * Функция проверяет, есть ли пространство справа от фигуры, чтобы она могла
 * продолжить движение вправо.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param field Двумерный массив игрового поля.
 *
 * @return true, если фигура может двигаться вправо; false, если она не может.
 */
bool canMoveRight(Piece *piece, int **field);

/**
 * @brief Проверяет, является ли фигура квадратной.
 *
 * Функция определяет, является ли фигура квадратной (т.е. занимает 2x2 клетки).
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 *
 * @return true, если фигура квадратная; false в противном случае.
 */
bool isSquarePiece(Piece *piece);

/**
 * @brief Поворачивает матрицу на 90 градусов по часовой стрелке.
 *
 * Функция выполняет поворот переданной матрицы на 90 градусов.
 *
 * @param src Исходная матрица (4x4).
 * @param dest Матрица (4x4), куда будет записан результат поворота.
 */
void rotateMatrix90(int src[4][4], int dest[4][4]);

/**
 * @brief Проверяет, допустим ли поворот фигуры.
 *
 * Функция проверяет, может ли фигура быть повернута в текущем положении без
 * столкновений с другими блоками или границами игрового поля.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param rotated Повернутая версия матрицы фигуры.
 * @param offsetX Смещение по оси X для проверки.
 * @param offsetY Смещение по оси Y для проверки.
 * @param field Двумерный массив игрового поля.
 *
 * @return true, если поворот возможен; false, если он недопустим.
 */
bool isRotationValid(Piece *piece, int rotated[4][4], int offsetX, int offsetY,
                     int **field);

/**
 * @brief Поворачивает фигуру.
 *
 * Функция изменяет ориентацию фигуры, если это возможно, и проверяет
 * на допустимость поворота.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param field Двумерный массив игрового поля.
 */
void rotatePiece(Piece *piece, int **field);

/**
 * @brief Применяет поворот фигуры с учётом смещения.
 *
 * Функция выполняет реальный поворот фигуры с применением смещений
 * по осям X и Y.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 * @param rotated Повернутая версия матрицы фигуры.
 * @param offsetX Смещение по оси X.
 * @param offsetY Смещение по оси Y.
 */
void applyRotation(Piece *piece, int rotated[4][4], int offsetX, int offsetY);

#endif  // PIECE_MOVE_H
