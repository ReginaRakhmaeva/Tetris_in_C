#include <stdlib.h>

#include "../../gui/cli/cli_interface.h"
int loadHighScore() {
  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  if (!file) {
    return 0;
  }
  int highScore = 0;
  fscanf(file, "%d", &highScore);
  fclose(file);
  return highScore;
}

void saveHighScore(int highScore) {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", highScore);
    fclose(file);
  }
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

void updateScoreAndLevel(GameInfo_t *game, int linesCleared) {
  game->score += linesCleared * 100;
  if (game->score > game->high_score) {
    game->high_score = game->score;
    saveHighScore(game->high_score);
  }
  if (game->level < 10) game->level = game->score / 600 + 1;
  game->speed = 1 + game->level;
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
