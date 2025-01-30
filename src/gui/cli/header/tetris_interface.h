/**
 * @file tetris_interface.h
 * @brief Заголовочный файл с функциями отрисовки интерфейса игры.
 */

#ifndef TETRIS_INTERFACE_H
#define TETRIS_INTERFACE_H

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../brick_game/tetris/header/game_logic.h"
#include "main.h"

/**
 * @brief Отображает стартовый экран.
 */
void renderStartScreen();

/**
 * @brief Отрисовывает игровое поле и текущие элементы.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую текущее состояние
 * игры.
 */
void drawField(GameInfo_t *game);

/**
 * @brief Отображает статическое (закрепленное) игровое поле.
 *
 * @param field Двумерный массив игрового поля.
 */
void drawStaticField(int **field);

/**
 * @brief Отрисовывает текущую фигуру.
 *
 * @param piece Указатель на структуру Piece, представляющую фигуру.
 */
void drawPiece(Piece *piece);

/**
 * @brief Отображает информацию об игре (очки, уровень и т. д.).
 *
 * @param game Указатель на структуру GameInfo_t.
 */
void drawGameInfo(GameInfo_t *game);

/**
 * @brief Очищает игровое поле.
 */
void clearField();

/**
 * @brief Отображает экран окончания игры.
 *
 * @param game Указатель на структуру GameInfo_t.
 */
void displayGameOverScreen(GameInfo_t *game);

/**
 * @brief Обрабатывает завершение игры, освобождая ресурсы.
 *
 * @param game Указатель на структуру GameInfo_t.
 * @param currentPiece Указатель на текущую падающую фигуру.
 */
void handleGameOver(GameInfo_t *game, Piece *currentPiece);

/**
 * @brief Обрабатывает ввод пользователя во время игры.
 *
 * @return Код нажатой клавиши.
 */
int processUserInput();

#endif  // TETRIS_INTERFACE_H
