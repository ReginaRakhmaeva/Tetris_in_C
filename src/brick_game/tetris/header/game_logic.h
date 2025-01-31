/**
 * @file game_logic.h
 * @brief Заголовочный файл, содержащий логику игры, включая обработку счета,
 * уровней и управления фигурой.
 */

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdlib.h>
#include <time.h>

#include "game_info.h"

/**
 * @brief Загружает рекордный счет из файла.
 * @return Рекордный счет (0, если файл не найден).
 */
int loadHighScore();

/**
 * @brief Сохраняет рекордный счет в файл.
 * @param highScore Рекордный счет для сохранения.
 */
void saveHighScore(int highScore);

/**
 * @brief Обновляет позицию текущей фигуры на поле.
 * @param currentPiece Указатель на текущую фигуру.
 * @param game Указатель на структуру GameInfo_t.
 * @param lastTick Указатель на время последнего обновления.
 * @return true, если фигура может двигаться дальше, иначе false.
 */
bool updatePiecePosition(Piece *currentPiece, GameInfo_t *game,
                         clock_t *lastTick);

/**
 * @brief Инициализирует начальные параметры игры.
 * @param game Указатель на структуру GameInfo_t.
 */
void initializeGame(GameInfo_t *game);

/**
 * @brief Обновляет очки после очистки линий.
 * @param game Указатель на структуру GameInfo_t.
 * @param linesCleared Количество очищенных линий.
 */
void updateScore(GameInfo_t *game, int linesCleared);

/**
 * @brief Обновляет уровень и скорость на основе очков.
 * @param game Указатель на структуру GameInfo_t.
 */
void updateLevelAndSpeed(GameInfo_t *game);

/**
 * @brief Очищает заполненные линии на игровом поле.
 * @param field Двумерный массив, представляющий игровое поле.
 * @return Количество очищенных линий.
 */
int clearFullLines(int **field);

#include "piece_create.h"
#include "piece_fix.h"
#include "piece_move.h"

#endif  // GAME_LOGIC_H
