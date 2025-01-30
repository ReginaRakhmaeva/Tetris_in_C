/**
 * @file fsm.h
 * @brief Определение функций и структур для обработки состояний игры и ввода
 * пользователя.
 */

#ifndef FSM_H
#define FSM_H

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../brick_game/tetris/header/game_logic.h"
#include "main.h"

/**
 * @brief Основной игровой цикл.
 *
 * Управляет процессом игры, обновляет состояние и обрабатывает события.
 */
void game_loop();

/**
 * @brief Обрабатывает ввод пользователя.
 *
 * @param action Действие пользователя, представленное в виде перечисления
 * UserAction_t.
 * @param hold Флаг удержания клавиши (true - если удерживается, false -
 * одиночное нажатие).
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Обрабатывает пользовательский ввод.
 *
 * @return Код действия, соответствующий пользовательскому вводу.
 */
int processUserInput();

/**
 * @brief Получает текущее состояние игрового поля.
 *
 * @return Двумерный массив (матрица), представляющий игровое поле.
 */
int **getFieldMatrix();

#endif  // FSM_H
