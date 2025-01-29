#include "../../brick_game/tetris/game_logic.h"
#include "cli_interface.h"

/**
 * @brief Основная функция программы.
 *
 * Функция инициализирует библиотеку ncurses, устанавливает локаль и отображает
 * экран начала игры. Если игра начинается, запускается игровой цикл. Если
 * пользователь решает выйти, программа завершает работу.
 *
 * @return Возвращает код завершения программы (успех или ошибка).
 */
int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  if (!showStartScreen()) {
    cleanupNcursesstart();
  } else
    game_loop();

  return SUCCESS;
}

/**
 * @brief Обновляет состояние игры.
 *
 * Функция обновляет состояние игры, включая положение текущей фигуры, очищение
 * линий и обновление счета. Если фигура не может быть перемещена вниз, она
 * фиксируется, и начинается новая фигура. Также происходит проверка на
 * окончание игры.
 *
 * @return Обновленное состояние игры.
 */
GameInfo_t updateCurrentState() {
  GameInfo_t *game = getGameInfo();
  Piece *currentPiece = getCurrentPiece();
  static clock_t lastTick = 0;
  if (lastTick == 0) lastTick = clock();

  if (game->pause) {
  } else {
    if (!game->level) {
      initializeGame(game);
    }

    if (!updatePiecePosition(currentPiece, game, &lastTick)) {
      drawField(game);
      refresh();
      spawnNewPiece(&currentPiece);
      handleGameOver(game, currentPiece);
    }

    int linesCleared = clearFullLines(game->field);
    updateScoreAndLevel(game, linesCleared);
  }

  return *game;
}
