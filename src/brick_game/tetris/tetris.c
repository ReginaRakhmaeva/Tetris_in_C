#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 20
#define COLS 10
#define SUCCESS 0
#define WIN_INIT(size) initNcurses()
#define GET_USER_INPUT getch()

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
  if (!piece) {
    spawnNewPiece(&piece);  // Инициализация новой фигуры
  }
  return piece;
}
void game_loop();

bool canMoveDown(Piece *piece, int **field);
void userInput(UserAction_t action, bool hold);
bool showStartScreen() {
  clear();

  mvprintw(ROWS / 2 - 5, COLS / 2, "TETRIS GAME");
  mvprintw(ROWS / 2, COLS / 2 - 4, "Press ENTER to Start");
  mvprintw(ROWS / 2 + 1, COLS / 2, "or q to Quit");
  mvprintw(ROWS / 2 + 4, COLS / 2 + 2, "RULES");
  mvprintw(ROWS / 2 + 5, COLS / 2 - 3, "Arrow Keys: Move");
  mvprintw(ROWS / 2 + 6, COLS / 2 - 2, "Space: Rotate");
  mvprintw(ROWS / 2 + 7, COLS / 2 - 3, "p: Pause, q: Quit");

  refresh();

  while (true) {
    int ch = GET_USER_INPUT;
    if (ch == '\n') {        // Нажата клавиша Enter
      return true;           // Начать игру
    } else if (ch == 'q') {  // Нажата клавиша q
      return false;          // Выйти из игры
    }
  }
}

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
  mvprintw(6, COLS * 2 + 5, "Next:");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->next[i][j]) {
        mvprintw(6 + i, COLS * 2 + 11 + j * 2, "[]");
      }
    }
  }
  refresh();
}

GameInfo_t updateCurrentState() {
  GameInfo_t *game = getGameInfo();
  Piece *currentPiece = getCurrentPiece();

  if (game->pause) {
    return *game;  // Если игра на паузе, просто возвращаем текущее состояние
  }

  if (!game->level) {
    game->level = 1;
    game->score = 0;
    game->high_score = 0;
    game->speed = 20 + game->level * 0.5;
    game->pause = 0;

    game->field = (int **)malloc(sizeof(int *) * ROWS);
    for (int i = 0; i < ROWS; i++) {
      game->field[i] = (int *)calloc(COLS, sizeof(int));
    }
  }

  static clock_t lastTick = 0;
  if (lastTick == 0) lastTick = clock();

  double elapsed = (double)(clock() - lastTick) / CLOCKS_PER_SEC;
  if (elapsed >= 0.05 / game->speed) {
    lastTick = clock();
    if (canMoveDown(currentPiece, game->field)) {
      currentPiece->y++;
    } else {
      fixPiece(game->field, currentPiece);
      spawnNewPiece(&currentPiece);
      if (!canMoveDown(currentPiece, game->field)) {
        game->pause = 1;  // Игра на паузу, если больше нет места
      }
    }
  }

  int lines_cleared = clearFullLines(game->field);
  game->score += lines_cleared * 100;
  if (game->level < 10) game->level = game->score / 600 + 1;
  game->speed = 1 + game->level;

  return *game;
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

void rotatePiece(Piece *piece, int **field) {
  // Проверка на квадратную фигуру (она не вращается)
  if (piece->shape[1][1] == 1 && piece->shape[0][0] == 1 &&
      piece->shape[0][1] == 1 && piece->shape[1][0] == 1) {
    return;
  }

  // Поворот фигуры на 90 градусов по часовой стрелке
  int rotated[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      rotated[j][3 - i] = piece->shape[i][j];
    }
  }

  // Массив смещений SRS для проверки при коллизии
  const int offsets[5][2] = {
      {0, 0},   // Нет смещения
      {-1, 0},  // Смещение влево
      {1, 0},   // Смещение вправо
      {0, -1},  // Смещение вверх
      {0, 1}    // Смещение вниз
  };

  // Проверка возможности применения поворота с учетом смещений
  for (int k = 0; k < 5; k++) {
    bool valid = true;
    int offsetX = offsets[k][0];
    int offsetY = offsets[k][1];

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (rotated[i][j]) {
          int newX = piece->x + j + offsetX;
          int newY = piece->y + i + offsetY;
          if (newX < 0 || newX >= COLS || newY >= ROWS ||
              (newY >= 0 && field[newY][newX])) {
            valid = false;
            break;
          }
        }
      }
      if (!valid) break;
    }

    if (valid) {
      // Применяем вращение и смещение
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          piece->shape[i][j] = rotated[i][j];
        }
      }
      piece->x += offsetX;
      piece->y += offsetY;
      return;
    }
  }

  if (piece->x + 3 >= COLS) {
    if (canMoveLeft(piece, field)) {
      piece->x -= 1;
      rotatePiece(piece, field);
    }
  } else if (piece->x < 0) {
    if (canMoveRight(piece, field)) {
      piece->x += 1;
      rotatePiece(piece, field);
    }
  }
}
void userInput(UserAction_t action, bool hold) {
  (void)hold;  // Параметр, если не нужен

  GameInfo_t *game = getGameInfo();
  Piece *currentPiece = getCurrentPiece();

  if (game->pause && action != Pause) {
    return;
  }

  switch (action) {
    case Left:
      if (canMoveLeft(currentPiece, game->field)) {
        currentPiece->x--;
      }
      break;

    case Right:
      if (canMoveRight(currentPiece, game->field)) {
        currentPiece->x++;
      }
      break;

    case Down:
      if (canMoveDown(currentPiece, game->field)) {
        currentPiece->y++;
      } else {
        fixPiece(game->field, currentPiece);
        spawnNewPiece(&currentPiece);
        if (!canMoveDown(currentPiece, game->field)) {
          game->pause = 1;
        }
      }
      break;

    case Action:
      rotatePiece(currentPiece, game->field);
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

  for (int i = 0; i < 4; i++) {
    free(game->next[i]);
  }
  free(game->next);
}
void cleanupNcursesstart() {
  endwin();  // Завершение работы с ncurses
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

void game_loop() {
  GameInfo_t *game = getGameInfo();
  userInput(Start, false);  // Начинаем игру
  bool break_flag = true;
  int signal = 0;

  while (break_flag) {
    signal = GET_USER_INPUT;  // Получаем пользовательский ввод
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
        break_flag = false;  // Выход из игры
        break;
      default:
        break;
    }

    if (!game->pause) {
      *game = updateCurrentState();
      drawField(game);
    }
  }

  cleanupNcurses(game);  // Завершение работы
}
