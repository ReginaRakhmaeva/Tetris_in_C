/**
 * @file game_info.h
 * @brief Заголовочный файл с функциями для получения информации о текущем
 * состоянии игры.
 */

#ifndef GAME_INFO_H
#define GAME_INFO_H

#include <stdio.h>

#include "objects.h"
#include "piece_create.h"

/**
 * @brief Возвращает указатель на информацию о текущей игре.
 *
 * @return Указатель на структуру GameInfo_t, содержащую данные об игре.
 */
GameInfo_t *getGameInfo();

/**
 * @brief Возвращает указатель на текущую фигуру.
 *
 * @return Указатель на структуру Piece, представляющую текущую фигуру.
 */
Piece *getCurrentPiece();

#endif  // GAME_INFO_H
