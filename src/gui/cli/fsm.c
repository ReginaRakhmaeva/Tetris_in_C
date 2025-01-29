#include "fsm.h"

#include "../../brick_game/tetris/game_logic.h"
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

    default:
      break;
  }
}

/**
 * @brief Обрабатывает ввод пользователя.
 *
 * @return Код ввода пользователя.
 */
int processUserInput() { return GET_USER_INPUT; }