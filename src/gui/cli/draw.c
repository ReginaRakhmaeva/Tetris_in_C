/**
 * @file draw.c
 * @brief Файл содержит функции для отображения игрового поля, интерфейса и
 * управления ncurses.
 *
 * Этот файл реализует функции для инициализации и завершения работы с ncurses,
 * отрисовки игрового поля, текущей фигуры, информации о игре и экрана "Game
 * Over". Он также обрабатывает ввод с клавиатуры для взаимодействия с
 * пользователем.
 */
#include "../../brick_game/tetris/game_logic.h"
#include "cli_interface.h"

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
 * @brief Обрабатывает конец игры и обновляет поле.
 *
 * Функция проверяет, может ли текущая фигура двигаться вниз. Если нет, она
 * фиксирует фигуру на поле, очищает поле и отрисовывает статичное поле с
 * информацией о текущем состоянии игры.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 * @param currentPiece Указатель на текущую фигуру игры.
 */
void handleGameOver(GameInfo_t *game, Piece *currentPiece) {
  if (!canMoveDown(currentPiece, game->field)) {
    if (isSpaceAvailableForFullFix(game->field, currentPiece)) {
      fixPiece(game->field, currentPiece);
    } else {
      fixPartialPiece(game->field, currentPiece);
    }
    clearField();
    drawStaticField(game->field);
    drawGameInfo(game);
    refresh();
    napms(500);
    game->pause = true;
    ungetch('q');
  }
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
 * @brief Очищает игровое поле.
 *
 * Функция очищает экран и отрисовывает границы игрового поля.
 */
void clearField() {
  clear();
  for (int i = 0; i <= ROWS; i++) {
    mvprintw(i, 0, "|");
    mvprintw(i, COLS * 2 + 1, "|");
  }
  for (int j = 0; j <= COLS * 2; j++) {
    mvprintw(0, j, "-");
    mvprintw(ROWS + 1, j, "-");
  }
  mvprintw(0, 0, "+");
  mvprintw(0, COLS * 2 + 1, "+");
  mvprintw(ROWS + 1, 0, "+");
  mvprintw(ROWS + 1, COLS * 2 + 1, "+");
}

/**
 * @brief Отрисовывает статичное игровое поле.
 *
 * Функция отрисовывает блоки на поле, которые уже заняты, отображая их как
 * "[]".
 *
 * @param field Игровое поле, представленное как двумерный массив.
 */
void drawStaticField(int **field) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (field[i][j]) {
        mvprintw(i + 1, j * 2 + 1, "[]");
      }
    }
  }
}

/**
 * @brief Отрисовывает текущую фигуру на игровом поле.
 *
 * Функция отрисовывает текущую фигуру на экране в соответствии с ее
 * координатами.
 *
 * @param piece Указатель на структуру Piece, представляющую текущую фигуру.
 */
void drawPiece(Piece *piece) {
  if (!piece) return;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        mvprintw(piece->y + i + 1, (piece->x + j) * 2 + 1, "[]");
      }
    }
  }
}

/**
 * @brief Отрисовывает информацию о текущей игре.
 *
 * Функция отображает информацию о текущем счете, уровне, высоком счете и
 * следующей фигуре.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void drawGameInfo(GameInfo_t *game) {
  mvprintw(2, COLS * 2 + 5, "Score: %d", game->score);
  mvprintw(3, COLS * 2 + 5, "Level: %d", game->level);
  mvprintw(4, COLS * 2 + 5, "High Score: %d", game->high_score);
  mvprintw(6, COLS * 2 + 5, "Next:");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->next[i][j]) {
        mvprintw(6 + i, COLS * 2 + 11 + j * 2, "[]");
      }
    }
  }
}

/**
 * @brief Отображает экран начала игры с инструкциями.
 *
 * Функция очищает экран и отображает название игры, инструкции и клавиши
 * управления.
 */
void renderStartScreen() {
  clear();

  mvprintw(ROWS / 2 - 5, COLS / 2, "TETRIS GAME");
  mvprintw(ROWS / 2, COLS / 2 - 4, "Press ENTER to Start");
  mvprintw(ROWS / 2 + 1, COLS / 2, "or q to Quit");
  mvprintw(ROWS / 2 + 4, COLS / 2 + 2, "RULES");
  mvprintw(ROWS / 2 + 5, COLS / 2 - 3, "Arrow Keys: Move");
  mvprintw(ROWS / 2 + 6, COLS / 2 - 2, "Space: Rotate");
  mvprintw(ROWS / 2 + 7, COLS / 2 - 3, "p: Pause, q: Quit");

  refresh();
}

/**
 * @brief Показывает экран начала игры и обрабатывает ввод.
 *
 * Функция отображает экран начала игры и обрабатывает пользовательский ввод для
 * начала игры или выхода.
 *
 * @return Возвращает true, если игра должна начаться, иначе false.
 */
bool showStartScreen() {
  renderStartScreen();
  return handleStartScreenInput();
}

/**
 * @brief Отрисовывает игровое поле, фигуру и информацию о текущем состоянии
 * игры.
 *
 * Функция очищает экран, отрисовывает статичное поле, текущую фигуру и
 * информацию о текущей игре.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void drawField(GameInfo_t *game) {
  clearField();
  drawStaticField(game->field);
  drawPiece(getCurrentPiece());
  drawGameInfo(game);
  refresh();
}

/**
 * @brief Отображает экран окончания игры.
 *
 * Функция очищает экран и отображает сообщение о завершении игры, а также
 * информацию о счете.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую информацию о
 * текущей игре.
 */
void displayGameOverScreen(GameInfo_t *game) {
  clear();
  mvprintw(ROWS / 2 - 1, COLS / 2 - 5, "GAME OVER");
  mvprintw(ROWS / 2, COLS / 2 - 8, "Your Score: %d", game->score);
  mvprintw(ROWS / 2 + 1, COLS / 2 - 10, "Press ENTER to Restart");
  mvprintw(ROWS / 2 + 2, COLS / 2 - 7, "or q to Quit");

  refresh();
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

  bool continueGame = true;
  while (continueGame) {
    continueGame = handleGameOverInput();
  }

  cleanupNcurses(game);
}
