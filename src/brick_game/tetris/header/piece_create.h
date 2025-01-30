/**
 * @file piece_create.h
 * @brief Функции для создания и управления фигурами Тетриса.
 */

#ifndef PIECE_CREATE_H
#define PIECE_CREATE_H

#include "game_logic.h"

/**
 * @brief Создает новую фигуру и подготавливает следующую.
 *
 * @param piece Указатель на указатель текущей фигуры.
 */
void spawnNewPiece(Piece **piece);

/**
 * @brief Инициализирует следующую фигуру (`next`), если она не задана.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую игровую информацию.
 * @param shapes Массив возможных фигур.
 */
void initializeNext(GameInfo_t *game, const int shapes[7][4][4]);

/**
 * @brief Создает текущую фигуру на основе `next`.
 *
 * @param piece Указатель на указатель структуры Piece.
 * @param game Указатель на структуру GameInfo_t.
 */
void createCurrentPiece(Piece **piece, GameInfo_t *game);

/**
 * @brief Обновляет следующую фигуру (`next`) новой случайной фигурой.
 *
 * @param game Указатель на структуру GameInfo_t.
 * @param shapes Массив возможных фигур.
 */
void updateNext(GameInfo_t *game, const int shapes[7][4][4]);

#endif  // PIECE_CREATE_H
