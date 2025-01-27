#include "../../brick_game/tetris/game_logic.h"
#include "cli_interface.h"
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

int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  if (!showStartScreen()) {
    cleanupNcursesstart();
  } else
    game_loop();

  return SUCCESS;
}

GameInfo_t updateCurrentState() {
  GameInfo_t *game = getGameInfo();
  Piece *currentPiece = getCurrentPiece();
  static clock_t lastTick = 0;
  if (lastTick == 0) lastTick = clock();

  if (game->pause) {
    return *game;  // Игра на паузе
  }

  if (!game->level) {
    initializeGame(game);  // Инициализация игры
  }

  if (!updatePiecePosition(currentPiece, game, &lastTick)) {
    drawField(game);
    refresh();
    spawnNewPiece(&currentPiece);
    handleGameOver(game, currentPiece);
  }

  int linesCleared = clearFullLines(game->field);
  updateScoreAndLevel(game, linesCleared);

  return *game;
}
