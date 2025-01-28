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
    return *game;
  }

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

  return *game;
}
