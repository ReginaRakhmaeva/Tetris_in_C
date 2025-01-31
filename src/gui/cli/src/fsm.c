/**
 * @file fsm.c
 * @brief Реализация конечного автомата для управления состоянием игры.
 *
 * Этот файл содержит реализацию игрового цикла, обработки пользовательского
 * ввода и управления состоянием игрового поля. Используется для обработки
 * событий и переходов между различными состояниями игры, такими как движение
 * фигур, пауза и завершение.
 */
#include "../header/fsm.h"

/**
 * @brief Главный игровой цикл.
 *
 * Функция запускает основной игровой цикл, обрабатывая пользовательский ввод и
 * обновляя состояние игры. В начале игра ожидает нажатия клавиши "Enter" для
 * старта. В зависимости от ввода (например, движение, вращение фигуры, пауза)
 * вызываются соответствующие функции.
 *
 * Управление:
 * - Стрелки ← → ↓: движение фигуры.
 * - Пробел: вращение фигуры.
 * - P: пауза.
 * - Q: выход (если игра не началась — завершает работу интерфейса).
 * - Enter: запуск игры.
 *
 * Цикл продолжается, пока игра не будет завершена или поставлена на паузу.
 */

#include <ncurses.h>  // Для работы с ncurses
#include <stdio.h>    // Добавляем для вывода отладки
#include <time.h>

void game_loop() {
  GameInfo_t *game = getGameInfo();
  bool break_flag = true;
  int fl_start = false;
  while (break_flag) {
    int signal = processUserInput();
    switch (signal) {
      case KEY_LEFT:
        userInput(Left, false);
        break;
      case KEY_RIGHT:
        userInput(Right, false);
        break;
      case KEY_DOWN:
        userInput(Down, false);
        break;
      case ' ':
        userInput(Action, false);
        break;
      case 'p':
        userInput(Pause, false);
        break;
      case 'q':
        if (fl_start) {
          userInput(Terminate, false);
        } else {
          cleanupNcursesstart();
        }
        break_flag = false;
        break;
      case '\n':
        if (!fl_start) userInput(Start, false);
        fl_start = true;
        break;
      default:
        break;
    }
    if (!game->pause && break_flag && fl_start) {
      *game = updateCurrentState();
      if (!game->pause) drawField(game);
      game->pause = false;
    }
  }
}

/**
 * @brief Обрабатывает ввод пользователя и выполняет соответствующие действия.
 *
 * Функция принимает действие пользователя (например, движение или вращение
 * фигуры) и выполняет его, если это возможно. Если игра на паузе, действия,
 * кроме паузы и завершения, игнорируются.
 *
 * Доступные действия:
 * - `Left`  — сдвиг фигуры влево.
 * - `Right` — сдвиг фигуры вправо.
 * - `Down`  — ускоренное падение фигуры.
 * - `Action` — вращение фигуры.
 * - `Pause` — постановка игры на паузу или её снятие.
 * - `Terminate` — завершение игры и вывод экрана завершения.
 * - `Start` — начало новой игры, обновление состояния и отрисовка поля.
 *
 * @param action Действие пользователя, представленное значением из перечисления
 * `UserAction_t`.
 * @param hold Зарезервированный параметр (не используется в текущей
 * реализации).
 */

void userInput(UserAction_t action, bool hold) {
  (void)hold;

  GameInfo_t *game = getGameInfo();
  Piece *currentPiece = getCurrentPiece();

  if (game->pause && action != Pause && action != Terminate) {
    return;
  }

  switch (action) {
    case Left:
      if (canMoveLeft(currentPiece, game->field)) {
        currentPiece->x--;
      }
      break;

    case Right:
      if (canMoveRight(currentPiece, game->field)) {
        currentPiece->x++;
      }
      break;

    case Down:
      if (canMoveDown(currentPiece, game->field)) {
        currentPiece->y++;
      }
      break;

    case Action:
      rotatePiece(currentPiece, game->field);
      break;

    case Up:
      break;

    case Pause:
      game->pause = !game->pause;
      break;

    case Terminate:
      showGameOverScreen(game);
      break;

    case Start:
      *game = updateCurrentState();
      drawField(game);
      break;

    default:
      break;
  }
}

/**
 * @brief Обрабатывает ввод пользователя.
 *
 * Функция считывает ввод пользователя и возвращает соответствующий код
 * действия.
 *
 * @return Код ввода пользователя (например, клавиши управления движением).
 */
int processUserInput() { return GET_USER_INPUT; }

/**
 * @brief Получает матрицу игрового поля.
 *
 * Функция возвращает указатель на текущую матрицу игрового поля.
 *
 * @return Двумерный массив, представляющий игровое поле.
 */
int **getFieldMatrix() {
  GameInfo_t *game = getGameInfo();
  return game->field;
}