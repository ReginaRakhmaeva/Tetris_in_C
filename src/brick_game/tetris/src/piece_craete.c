/**
 * @file piece_create.c
 * @brief Функции для создания фигур в игре Тетрис.
 *
 * Этот файл содержит функции для генерации новых фигур,
 * управления следующей фигурой и инициализации текущей фигуры.
 */

#include "../header/piece_create.h"
/**
 * @brief Основная функция для создания новой фигуры.
 *
 * @param piece Указатель на указатель структуры Piece, представляющей фигуру.
 */
void spawnNewPiece(Piece **piece) {
  static const int shapes[7][4][4] = {
      {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Линия
      {{1, 1}, {1, 1}, {0, 0}, {0, 0}},              // Квадрат
      {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}, {0, 0, 0}},  // Z-образная
      {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}, {0, 0, 0}},  // Обратная Z
      {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}},  // T-образная
      {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}},  // L-образная
      {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}}   // Обратная L
  };

  GameInfo_t *game = getGameInfo();

  initializeNext(game, shapes);
  createCurrentPiece(piece, game);
  updateNext(game, shapes);
}
/**
 * @brief Инициализирует следующую фигуру (`next`), если она не задана.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую игровую информацию.
 * @param shapes Массив фигур, представленных в виде матриц.
 */
void initializeNext(GameInfo_t *game, const int shapes[7][4][4]) {
  if (!game->next) {
    game->next = (int **)malloc(sizeof(int *) * 4);
    for (int i = 0; i < 4; i++) {
      game->next[i] = (int *)calloc(4, sizeof(int));
    }
    int shapeIndex = rand() % 7;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        game->next[i][j] = shapes[shapeIndex][i][j];
      }
    }
  }
}

/**
 * @brief Создаёт текущую фигуру из следующей (`next`).
 *
 * @param piece Указатель на указатель структуры Piece, представляющей фигуру.
 * @param game Указатель на структуру GameInfo_t, содержащую игровую информацию.
 */
void createCurrentPiece(Piece **piece, GameInfo_t *game) {
  *piece = (Piece *)malloc(sizeof(Piece));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*piece)->shape[i][j] = game->next[i][j];
    }
  }
  (*piece)->x = COLS / 2 - 2;
  (*piece)->y = 0;
}

/**
 * @brief Обновляет следующую фигуру (`next`) новой случайной фигурой.
 *
 * @param game Указатель на структуру GameInfo_t, содержащую игровую информацию.
 * @param shapes Массив фигур, представленных в виде матриц.
 */
void updateNext(GameInfo_t *game, const int shapes[7][4][4]) {
  int shapeIndex = rand() % 7;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next[i][j] = shapes[shapeIndex][i][j];
    }
  }
}
