/**
 * @file main.c
 * @brief Главный файл игры.
 *
 * Этот файл содержит основную точку входа в игру, а также функции для
 * инициализации и завершения работы с ncurses, отображения стартового экрана,
 * игрового цикла и экрана окончания игры. Он управляет основным потоком
 * выполнения программы и обновлением состояния игры.
 */

#include "../header/main.h"

#include "../../../brick_game/tetris/header/defines.h"
#include "../../../brick_game/tetris/header/game_logic.h"

/**
 * @brief Основная функция программы.
 *
 * Функция инициализирует библиотеку ncurses, устанавливает локаль.
 * Отображает стартовый экран с приглашением начать игру (`renderStartScreen`).
 * Запускает основной игровой цикл (`game_loop`).
 *
 * @return Возвращает код завершения программы (успех или ошибка).
 */
int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  renderStartScreen();
  game_loop();

  return SUCCESS;
}
/**
 * @brief Инициализирует библиотеку ncurses для отображения игры в консоли.
 *
 * Функция настраивает ncurses для работы с терминалом: отключает буферизацию
 * ввода, скрывает курсор, устанавливает режим обработки клавиш и задает таймаут
 * для обновления экрана.
 */
void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(50);
}

/**
 * @brief Завершает работу с ncurses и очищает память, выделенную для поля и
 * фигур.
 *
 * Функция завершает работу с ncurses и освобождает память, выделенную для
 * игрового поля и фигур.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void cleanupNcurses(GameInfo_t *game) {
  endwin();
  for (int i = 0; i < ROWS; i++) {
    free(game->field[i]);
  }
  free(game->field);

  for (int i = 0; i < 4; i++) {
    free(game->next[i]);
  }
  free(game->next);
}

/**
 * @brief Завершает работу с ncurses без очистки данных игры.
 *
 * Функция завершает работу с ncurses без освобождения памяти, связанной с
 * игровыми данными.
 */
void cleanupNcursesstart() {
  endwin();  // Завершение работы с ncurses
}

/**
 * @brief Показывает экран окончания игры и обрабатывает ввод.
 *
 * Функция отображает экран окончания игры и обрабатывает пользовательский ввод
 * для перезапуска игры или выхода.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void showGameOverScreen(GameInfo_t *game) {
  displayGameOverScreen(game);
  napms(1000);
  cleanupNcurses(game);
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
    updateScore(game, linesCleared);
    updateLevelAndSpeed(game);
  }

  return *game;
}
