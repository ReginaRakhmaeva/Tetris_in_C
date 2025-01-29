/**
 * @file game_logic.c
 * @brief Основная логика игры Тетрис, включая обработку уровней, очков и
 * управления игровым полем.
 */

#include "game_logic.h"

#include <stdlib.h>

/**
 * @brief Загружает рекордный счёт из файла.
 *
 * @return Рекордный счёт (0, если файл недоступен).
 */
int loadHighScore() {
  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  int highScore = 0;
  if (file) {
    fscanf(file, "%d", &highScore);
    fclose(file);
  }
  return highScore;
}

/**
 * @brief Сохраняет рекордный счёт в файл.
 *
 * @param highScore Рекордный счёт для сохранения.
 */
void saveHighScore(int highScore) {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", highScore);
    fclose(file);
  }
}

/**
 * @brief Инициализирует начальные параметры игры.
 *
 * @param game Указатель на структуру GameInfo_t, представляющую состояние игры.
 */
void initializeGame(GameInfo_t *game) {
  game->level = 1;
  game->score = 0;
  game->high_score = loadHighScore();
  game->speed = 20 + game->level * 0.5;
  game->pause = 0;

  game->field = (int **)malloc(sizeof(int *) * ROWS);
  for (int i = 0; i < ROWS; i++) {
    game->field[i] = (int *)calloc(COLS, sizeof(int));
  }
}

/**
 * @brief Обновляет позицию текущей фигуры на игровом поле.
 *
 * @param currentPiece Указатель на текущую фигуру.
 * @param game Указатель на структуру GameInfo_t.
 * @param lastTick Указатель на время последнего обновления.
 *
 * @return true, если фигура может двигаться вниз; false, если фигура
 * зафиксирована.
 */
bool updatePiecePosition(Piece *currentPiece, GameInfo_t *game,
                         clock_t *lastTick) {
  bool canContinue = true;
  double elapsed = (double)(clock() - *lastTick) / CLOCKS_PER_SEC;

  if (elapsed >= 0.04 / game->speed) {
    *lastTick = clock();
    if (canMoveDown(currentPiece, game->field)) {
      currentPiece->y++;
    } else {
      fixPiece(game->field, currentPiece);
      canContinue = false;
    }
  }
  return canContinue;
}

/**
 * @brief Обновляет очки и уровень игрока после очистки линий.
 *
 * @param game Указатель на структуру GameInfo_t.
 * @param linesCleared Количество очищенных линий.
 */
void updateScoreAndLevel(GameInfo_t *game, int linesCleared) {
  switch (linesCleared) {
    case 1:
      game->score += 100;
      break;
    case 2:
      game->score += 300;
      break;
    case 3:
      game->score += 700;
      break;
    case 4:
      game->score += 1500;
      break;
    default:
      break;
  }
  if (game->score > game->high_score) {
    game->high_score = game->score;
    saveHighScore(game->high_score);
  }
  if (game->level < 10) game->level = game->score / 600 + 1;
  game->speed = game->level;
}

/**
 * @brief Очищает заполненные линии на игровом поле.
 *
 * @param field Двумерный массив, представляющий игровое поле.
 *
 * @return Количество очищенных линий.
 */
int clearFullLines(int **field) {
  int cleared = 0;

  for (int i = 0; i < ROWS; i++) {
    bool full = true;
    for (int j = 0; j < COLS && full; j++) {
      if (!field[i][j]) {
        full = false;
      }
    }

    if (full) {
      cleared++;
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < COLS; j++) {
          field[k][j] = field[k - 1][j];
        }
      }
      for (int j = 0; j < COLS; j++) {
        field[0][j] = 0;
      }
    }
  }
  return cleared;
}

/**
 * @brief Проверяет, доступно ли место для полной фиксации фигуры на поле.
 *
 * @param field Двумерный массив, представляющий игровое поле.
 * @param piece Указатель на фигуру.
 *
 * @return true, если место доступно; false, если место недоступно.
 */
bool isSpaceAvailableForFullFix(int **field, Piece *piece) {
  bool isAvailable = true;

  for (int i = 0; i < 4 && isAvailable; i++) {
    for (int j = 0; j < 4 && isAvailable; j++) {
      if (piece->shape[i][j]) {
        int newX = piece->x + j;
        int newY = piece->y + i;
        if (newY < 0 || newY >= ROWS || newX < 0 || newX >= COLS ||
            field[newY][newX]) {
          isAvailable = false;
        }
      }
    }
  }
  return isAvailable;
}
