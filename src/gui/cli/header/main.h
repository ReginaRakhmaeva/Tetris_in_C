/**
 * @file main.h
 * @brief Заголовочный файл с основными функциями для инициализации и управления
 * игрой.
 */

#ifndef MAIN_H
#define MAIN_H

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../brick_game/tetris/header/game_logic.h"
#include "fsm.h"
#include "tetris_interface.h"

/**
 * @brief Обновляет текущее состояние игры.
 *
 * @return Структура GameInfo_t с обновленными параметрами игры.
 */
GameInfo_t updateCurrentState();

/**
 * @brief Инициализирует ncurses для работы с текстовым интерфейсом.
 */
void initNcurses();

/**
 * @brief Очищает ресурсы ncurses перед завершением работы.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию об игре.
 */
void cleanupNcurses(GameInfo_t *game);

/**
 * @brief Очищает ncurses перед выходом из стартового экрана.
 */
void cleanupNcursesstart();

/**
 * @brief Отображает экран "Game Over".
 *
 * @param game Указатель на структуру GameInfo_t для доступа к состоянию игры.
 */
void showGameOverScreen(GameInfo_t *game);

#endif  // MAIN_H
