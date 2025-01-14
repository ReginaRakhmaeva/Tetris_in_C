#include "fsm.h"

// Реализация функций управления состояниями
void game_loop() {
  GameInfo_t *game = getGameInfo();
  userInput(Start, false);  // Начинаем игру
  bool break_flag = true;
  int signal = 0;

  while (break_flag) {
    signal = GET_USER_INPUT;  // Получаем пользовательский ввод
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
        userInput(Action, false);  // Пробел для поворота фигуры
        break;
      case 'p':
        userInput(Pause, false);  // Пауза
        break;
      case 'q':
        userInput(Terminate, false);
        break_flag = false;  // Выход из игры
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

void userInput(UserAction_t action, bool hold) {
  (void)hold;  // Параметр, если не нужен

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
