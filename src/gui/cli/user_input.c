/**
 * @file user_input.c
 * @brief Файл содержит функции для обработки ввода пользователя в игре.
 *
 * Этот файл реализует функции для обработки действий пользователя, таких как
 * движение фигуры, вращение, пауза, а также обработку ввода на экранах начала
 * игры и окончания игры.
 */

#include "../../brick_game/tetris/game_logic.h"
#include "cli_interface.h"
/**
 * @brief Главный игровой цикл.
 *
 * Функция запускает основной игровой цикл, обрабатывая пользовательский ввод и
 * обновляя состояние игры. В зависимости от ввода (например, движение, вращение
 * фигуры, пауза) вызываются соответствующие функции. Цикл продолжается, пока
 * игра не будет завершена или поставлена на паузу.
 */
void game_loop() {
  GameInfo_t *game = getGameInfo();
  userInput(Start, false);  // Начинаем игру
  bool break_flag = true;

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
        userInput(Terminate, false);
        break_flag = false;
        break;
      default:
        break;
    }

    if (!game->pause && break_flag) {
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
 * кроме паузы, не выполняются.
 *
 * @param action Действие пользователя, представленное значением из перечисления
 * UserAction_t.
 * @param hold Параметр, который не используется в данной функции, но может быть
 * использован в будущем.
 */

void userInput(UserAction_t action, bool hold) {
  (void)hold;

  GameInfo_t *game = getGameInfo();
  Piece *currentPiece = getCurrentPiece();

  if (game->pause && action != Pause) {
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

    default:
      break;
  }
}

/**
 * @brief Обрабатывает ввод пользователя на экране начала игры.
 *
 * Функция ожидает ввод пользователя на экране начала игры. Если пользователь
 * нажимает клавишу Enter, игра начинается. Если нажата клавиша 'q', игра
 * завершается.
 *
 * @return Возвращает true, если игра должна начаться, иначе false.
 */
bool handleStartScreenInput() {
  while (true) {
    int ch = processUserInput();
    if (ch == '\n') {        // Нажата клавиша Enter
      return true;           // Начать игру
    } else if (ch == 'q') {  // Нажата клавиша q
      return false;          // Выйти из игры
    }
  }
}

/**
 * @brief Обрабатывает ввод пользователя на экране окончания игры.
 *
 * Функция ожидает ввод пользователя на экране окончания игры. Если пользователь
 * нажимает клавишу 'q', игра завершается.
 *
 * @return Возвращает true, если игра должна продолжиться, иначе false.
 */
bool handleGameOverInput() {
  int ch = processUserInput();
  int fl = true;
  if (ch == 'q') {
    fl = false;
  }
  return fl;
}

/**
 * @brief Обрабатывает ввод пользователя.
 *
 * @return Код ввода пользователя.
 */
int processUserInput() { return GET_USER_INPUT; }