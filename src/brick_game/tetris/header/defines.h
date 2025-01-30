/**
 * @file defines.h
 * @brief Определение макросов, используемых в игре.
 */

#ifndef DEFINES_H
#define DEFINES_H

/** @brief Путь к файлу с рекордным счетом. */
#define HIGH_SCORE_FILE "brick_game/tetris/highscore.txt"

/** @brief Количество строк в игровом поле. */
#define ROWS 20

/** @brief Количество столбцов в игровом поле. */
#define COLS 10

/** @brief Код успешного выполнения. */
#define SUCCESS 0

/** @brief Макрос для получения ввода пользователя (getch из ncurses). */
#define GET_USER_INPUT getch()

/** @brief Макрос для инициализации ncurses. */
#define WIN_INIT(size) initNcurses()

#endif  // DEFINES_H
