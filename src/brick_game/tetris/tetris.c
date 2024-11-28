#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 20
#define COLS 10

// перечисление набора именнованных констант
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

// информация о текущем состоянии игры
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  int x;
  int y;
  int shape[4][4];
} Piece;

GameInfo_t *getGameInfo() {
  static GameInfo_t game;
  return &game;
}
void spawnNewPiece(Piece **piece);
Piece *getCurrentPiece() {
  static Piece *piece = NULL;
  if (piece == NULL) {
    spawnNewPiece(&piece);  // Инициализация фигуры при первом запросе
  }
  return piece;
}
bool canMoveDown(Piece *piece, int **field);
void userInput(UserAction_t action, bool hold);

bool checkCollision(int **field, Piece *piece);
GameInfo_t updateCurrentState();

void drawField(GameInfo_t *game);
int clearFullLines(int **field);
void fixPiece(int **field, Piece *piece);
void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(50);
}

void drawField(GameInfo_t *game) {
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

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (game->field[i][j]) {
        mvprintw(i + 1, j * 2 + 1, "[]");
      }
    }
  }

  Piece *currentPiece = getCurrentPiece();
  if (currentPiece) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (currentPiece->shape[i][j]) {
          mvprintw(currentPiece->y + i + 1, (currentPiece->x + j) * 2 + 1,
                   "[]");
        }
      }
    }
  }
  mvprintw(2, COLS * 2 + 5, "Score: %d", game->score);
  mvprintw(3, COLS * 2 + 5, "Level: %d", game->level);
  mvprintw(4, COLS * 2 + 5, "High Score: %d", game->high_score);

  refresh();
}

GameInfo_t updateCurrentState() {
  GameInfo_t *game = getGameInfo();
  static Piece *current_piece = NULL;

  if (!game->level) {
    game->level = 1;
    game->score = 0;
    game->high_score = 0;
    game->speed = 100;
    game->pause = 0;

    game->field = (int **)malloc(sizeof(int *) * ROWS);
    for (int i = 0; i < ROWS; i++) {
      game->field[i] = (int *)calloc(COLS, sizeof(int));
    }

    spawnNewPiece(&current_piece);
  }

  static int tick = 100;
  tick++;
  if (tick >= game->speed) {
    tick = 0;
    current_piece->y++;
    if (!canMoveDown(current_piece, game->field)) {
      current_piece->y--;
      fixPiece(game->field, current_piece);
      spawnNewPiece(&current_piece);
    }
  }

  int lines_cleared = clearFullLines(game->field);
  game->score += lines_cleared * 100;
  game->level = game->score / 1000 + 1;
  game->speed = 100 - game->level * 10;

  return *game;  // Возвращаем копию обновленного состояния
}

bool checkCollision(int **field, Piece *piece) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->shape[i][j]) {
        int newX = piece->x + j;
        int newY = piece->y + i;

        if (newX < 0 || newX >= COLS || newY >= ROWS || field[newY][newX]) {
          return true;
        }
      }
    }
  }
  return false;
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

  *piece = (Piece *)malloc(sizeof(Piece));
  int shapeIndex = rand() % 7;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*piece)->shape[i][j] = shapes[shapeIndex][i][j];
    }
  }

  (*piece)->x = COLS / 2 - 2;
  (*piece)->y = 0;
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

bool canRotate(Piece *piece, int **field) {
  int rotated[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      rotated[j][3 - i] = piece->shape[i][j];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (rotated[i][j]) {
        int newX = piece->x + j;
        int newY = piece->y + i;
        if (newX < 0 || newX >= COLS || newY >= ROWS || field[newY][newX]) {
          return false;
        }
      }
    }
  }
  return true;
}
void rotatePiece(Piece *piece) {
  if (piece->shape[1][1] == 1 && piece->shape[0][0] == 1 &&
      piece->shape[0][1] == 1 && piece->shape[1][0] == 1) {
    return;
  }
  int rotated[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      rotated[j][3 - i] = piece->shape[i][j];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      piece->shape[i][j] = rotated[i][j];
    }
  }
}
void userInput(UserAction_t action, bool hold) {
  (void)hold;  // не используется

  GameInfo_t *game = getGameInfo();
  Piece *currentPiece = getCurrentPiece();

  if (game->pause && action != Pause) {
    return;
  }

  switch (action) {
    case Left:
      if (currentPiece && canMoveLeft(currentPiece, game->field)) {
        currentPiece->x -= 1;
      }
      break;

    case Right:
      if (currentPiece && canMoveRight(currentPiece, game->field)) {
        currentPiece->x += 1;
      }
      break;

    case Down:
      if (currentPiece && canMoveDown(currentPiece, game->field)) {
        currentPiece->y += 1;
      } else {
        fixPiece(game->field, currentPiece);
        spawnNewPiece(&currentPiece);
        if (!canMoveDown(currentPiece, game->field)) {
          game->pause = 1;
        }
      }
      break;

    case Action:
      if (currentPiece && canRotate(currentPiece, game->field)) {
        rotatePiece(currentPiece);
      }
      break;

    case Pause:
      game->pause = !game->pause;
      break;

    default:
      break;
  }
}

void cleanupNcurses(GameInfo_t *game) {
  endwin();
  for (int i = 0; i < ROWS; i++) {
    free(game->field[i]);
  }
  free(game->field);
}

int main() {
  initNcurses();

  while (true) {
    GameInfo_t game = updateCurrentState();
    drawField(&game);
    int ch = getch();
    if (ch == 'q') {
      cleanupNcurses(&game);
      break;
    }
    switch (ch) {
      case KEY_LEFT:
        userInput(Left, false);
        break;
      case KEY_RIGHT:
        userInput(Right, false);
        break;
      case KEY_DOWN:
        userInput(Down, false);
        break;
      case KEY_UP:
        break;
      case ' ':
        userInput(Action, false);
        break;
      case 'p':
        userInput(Pause, false);
        break;
    }
  }

  return 0;
}
