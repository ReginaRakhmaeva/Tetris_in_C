#include "backend.h"

#include <stdlib.h>

// Чтение максимального счета из файла
int loadHighScore() {
  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  if (!file) {
    return 0;  // Если файл не найден, вернуть 0
  }
  int highScore = 0;
  fscanf(file, "%d", &highScore);
  fclose(file);
  return highScore;
}

// Сохранение максимального счета в файл
void saveHighScore(int highScore) {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", highScore);
    fclose(file);
  }
}

// Реализация функций логики
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

  // Если `next` пустой, создаем первую фигуру
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

  // Создаем текущую фигуру из `next`
  *piece = (Piece *)malloc(sizeof(Piece));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*piece)->shape[i][j] = game->next[i][j];
    }
  }
  (*piece)->x = COLS / 2 - 2;
  (*piece)->y = 0;

  // Обновляем `next`
  int shapeIndex = rand() % 7;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next[i][j] = shapes[shapeIndex][i][j];
    }
  }
}

bool canMoveDown(Piece *piece, int **field) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        if (piece->y + i + 1 >= ROWS || field[piece->y + i + 1][piece->x + j]) {
          return false;
        }
      }
    }
  }
  return true;
}

void fixPiece(int **field, Piece *piece) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        int newX = piece->x + j;
        int newY = piece->y + i;
        field[newY][newX] = 1;
      }
    }
  }
  free(piece);
}

bool canMoveLeft(Piece *piece, int **field) {
  if (piece == NULL) {
    return false;
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        if (piece->x + j - 1 < 0 || piece->y + i < 0 || piece->y + i >= ROWS ||
            piece->x + j - 1 >= COLS || field[piece->y + i][piece->x + j - 1]) {
          return false;
        }
      }
    }
  }
  return true;
}

bool canMoveRight(Piece *piece, int **field) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        if (piece->x + j + 1 >= COLS || field[piece->y + i][piece->x + j + 1]) {
          return false;
        }
      }
    }
  }
  return true;
}

bool isSpaceAvailableForFullFix(int **field, Piece *piece) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        int newX = piece->x + j;
        int newY = piece->y + i;
        if (newY < 0 || newY >= ROWS || newX < 0 || newX >= COLS ||
            field[newY][newX]) {
          return false;  // Место недоступно
        }
      }
    }
  }
  return true;  // Места достаточно
}
void fixPartialPiece(int **field, Piece *piece) {
  for (int i = 3; i >= 0; i--) {
    bool hasBlock = false;
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        hasBlock = true;
        int newX = piece->x + j;
        int newY = 0;
        if (newY >= 0 && newY < ROWS && newX >= 0 && newX < COLS) {
          field[newY][newX] = 1;
        }
      }
    }
    if (hasBlock) {
      break;
    }
  }
  free(piece);
}
int clearFullLines(int **field) {
  int cleared = 0;

  for (int i = 0; i < ROWS; i++) {
    bool full = true;
    for (int j = 0; j < COLS; j++) {
      if (!field[i][j]) {
        full = false;
        break;
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
bool updatePiecePosition(Piece *currentPiece, GameInfo_t *game,
                         clock_t *lastTick) {
  double elapsed = (double)(clock() - *lastTick) / CLOCKS_PER_SEC;
  if (elapsed >= 0.05 / game->speed) {
    *lastTick = clock();
    if (canMoveDown(currentPiece, game->field)) {
      currentPiece->y++;
      return true;
    } else {
      fixPiece(game->field, currentPiece);
      return false;
    }
  }
  return true;
}
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
void updateScoreAndLevel(GameInfo_t *game, int linesCleared) {
  game->score += linesCleared * 100;
  if (game->score > game->high_score) {
    game->high_score = game->score;
    saveHighScore(game->high_score);
  }
  if (game->level < 10) game->level = game->score / 600 + 1;
  game->speed = 1 + game->level;
}

bool isSquarePiece(Piece *piece) {
  return piece->shape[1][1] == 1 && piece->shape[0][0] == 1 &&
         piece->shape[0][1] == 1 && piece->shape[1][0] == 1;
}

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
void rotateMatrix90(int src[4][4], int dest[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest[j][3 - i] = src[i][j];
    }
  }
}

bool isRotationValid(Piece *piece, int rotated[4][4], int offsetX, int offsetY,
                     int **field) {
  bool isValid = true;

  for (int i = 0; i < 4 && isValid; i++) {
    for (int j = 0; j < 4 && isValid; j++) {
      if (rotated[i][j]) {
        int newX = piece->x + j + offsetX;
        int newY = piece->y + i + offsetY;

        if (newX < 0 || newX >= COLS || newY >= ROWS ||
            (newY >= 0 && field[newY][newX])) {
          isValid = false;
        }
      }
    }
  }
  return isValid;
}

void applyRotation(Piece *piece, int rotated[4][4], int offsetX, int offsetY) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      piece->shape[i][j] = rotated[i][j];
    }
  }
  piece->x += offsetX;
  piece->y += offsetY;
}

void rotatePiece(Piece *piece, int **field) {
  if (isSquarePiece(piece)) {
    return;  // Проверка аргументов
  }

  int rotated[4][4];
  rotateMatrix90(piece->shape, rotated);

  const int offsets[5][2] = {
      {0, 0},   // No offset
      {-1, 0},  // Left
      {1, 0},   // Right
      {0, -1},  // Up
      {0, 1}    // Down
  };

  bool rotationApplied = false;

  for (int k = 0; k < 5 && !rotationApplied; k++) {
    int offsetX = offsets[k][0];
    int offsetY = offsets[k][1];

    if (isRotationValid(piece, rotated, offsetX, offsetY, field)) {
      applyRotation(piece, rotated, offsetX, offsetY);
      rotationApplied = true;
    }
  }

  if (!rotationApplied) {
    if (piece->x + 3 >= COLS && canMoveLeft(piece, field)) {
      piece->x -= 1;
    } else if (piece->x < 0 && canMoveRight(piece, field)) {
      piece->x += 1;
    }
    rotatePiece(piece, field);  // Рекурсия для повторной попытки
  }
}
