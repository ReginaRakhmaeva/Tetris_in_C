#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../brick_game/tetris/game_logic.h"

// Функция для инициализации поля
int **initializeField() {
  int **field = (int **)malloc(sizeof(int *) * ROWS);
  for (int i = 0; i < ROWS; i++) {
    field[i] = (int *)calloc(COLS, sizeof(int));
  }
  return field;
}

void freeField(int **field) {
  for (int i = 0; i < ROWS; i++) {
    free(field[i]);
  }
  free(field);
}

START_TEST(test_can_move_down) {
  Piece *piece = getCurrentPiece();
  int **field = initializeField();

  piece->x = 5;
  piece->y = 4;
  piece->shape[1][1] = 1;

  ck_assert_int_eq(canMoveDown(piece, field), true);

  field[6][5] = 1;

  ck_assert_int_eq(canMoveDown(piece, field), false);

  freeField(field);
}
END_TEST

START_TEST(test_can_move_left) {
  Piece *piece = getCurrentPiece();
  int **field = initializeField();

  piece->x = 1;
  piece->y = 0;
  piece->shape[1][1] = 1;

  field[1][0] = 1;

  ck_assert_int_eq(canMoveLeft(piece, field), false);

  field[1][0] = 0;

  ck_assert_int_eq(canMoveLeft(piece, field), true);

  field[1][0] = 1;

  ck_assert_int_eq(canMoveLeft(piece, field), false);

  piece->x = 0;

  ck_assert_int_eq(canMoveLeft(piece, field), false);

  freeField(field);
}
END_TEST

START_TEST(test_can_move_right) {
  int **field = initializeField();
  Piece *piece = getCurrentPiece();

  piece->x = 5;
  piece->y = 4;
  piece->shape[1][1] = 1;

  ck_assert_int_eq(canMoveRight(piece, field), true);

  field[5][7] = 1;

  ck_assert_int_eq(canMoveRight(piece, field), false);

  piece->x = COLS - 2;
  ck_assert_int_eq(canMoveRight(piece, field), false);

  freeField(field);
}
END_TEST

int loadHighScore();

START_TEST(test_load_high_score) {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  ck_assert_ptr_nonnull(file);

  fprintf(file, "%d\n", 100);
  fclose(file);

  ck_assert_int_eq(loadHighScore(), 100);

  remove(HIGH_SCORE_FILE);

  ck_assert_int_eq(loadHighScore(), 0);
}
void saveHighScore(int highScore);
START_TEST(test_save_high_score) {
  int testScore = 1234;

  remove(HIGH_SCORE_FILE);

  saveHighScore(testScore);

  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  ck_assert_ptr_nonnull(file);

  int loadedScore;
  fscanf(file, "%d", &loadedScore);
  fclose(file);

  ck_assert_int_eq(loadedScore, testScore);

  remove(HIGH_SCORE_FILE);
}
END_TEST

// START_TEST(test_update_piece_position) {
//   GameInfo_t game = {
//       .field = initializeField(),
//       .score = 0,
//       .high_score = 0,
//       .level = 1,
//       .speed = 2,
//   };

//   Piece piece = {
//       .x = 4,
//       .y = 0,
//       .shape =
//           {
//               {1, 1, 0, 0},
//               {1, 1, 0, 0},
//               {0, 0, 0, 0},
//               {0, 0, 0, 0},
//           },
//   };

//   clock_t lastTick = clock();
//   while ((double)(clock() - lastTick) / CLOCKS_PER_SEC < 0.05 / game.speed) {
//   }

//   ck_assert(updatePiecePosition(&piece, &game, &lastTick) == true);
//   ck_assert_int_eq(piece.y, 1);

//   piece.y = ROWS - 2;
//   for (int i = 0; i < COLS; i++) {
//     game.field[ROWS - 1][i] = 1;
//   }

//   ck_assert_int_eq(piece.y, ROWS - 2);

//   lastTick = clock();
//   ck_assert_int_eq(piece.y, ROWS - 2);

//   game.speed = 4;
//   lastTick = clock() - CLOCKS_PER_SEC / 4;
//   ck_assert(updatePiecePosition(&piece, &game, &lastTick) == false);
//   ck_assert_int_eq(game.field[ROWS - 2][4], 1);

//   freeField(game.field);
// }
// END_TEST

// Тест для функции initializeGame
START_TEST(test_initialize_game) {
  GameInfo_t game;

  initializeGame(&game);

  ck_assert_int_eq(game.level, 1);
  ck_assert_int_eq(game.score, 0);
  ck_assert_int_eq(game.high_score, 0);
  ck_assert_float_eq(game.speed, 20);
  ck_assert_int_eq(game.pause, 0);

  ck_assert_ptr_nonnull(game.field);
  for (int i = 0; i < ROWS; i++) {
    ck_assert_ptr_nonnull(game.field[i]);
    for (int j = 0; j < COLS; j++) {
      ck_assert_int_eq(game.field[i][j], 0);
    }
  }

  for (int i = 0; i < ROWS; i++) {
    free(game.field[i]);
  }
  free(game.field);
}
END_TEST

START_TEST(test_fix_piece) {
  int **field = (int **)malloc(sizeof(int *) * ROWS);
  for (int i = 0; i < ROWS; i++) {
    field[i] = (int *)calloc(COLS, sizeof(int));
  }

  Piece *piece = (Piece *)malloc(sizeof(Piece));
  piece->x = 5;
  piece->y = 4;
  piece->shape[0][0] = 1;
  piece->shape[1][0] = 1;
  piece->shape[2][0] = 1;
  piece->shape[2][1] = 1;

  fixPiece(field, piece);

  ck_assert_int_eq(field[4][5], 1);
  ck_assert_int_eq(field[5][5], 1);
  ck_assert_int_eq(field[6][5], 1);
  ck_assert_int_eq(field[6][6], 1);

  freeField(field);
}
END_TEST

// Тест для isSquarePiece
START_TEST(test_isSquarePiece) {
  Piece squarePiece = {
      0, 0, {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};
  Piece nonSquarePiece = {
      0, 0, {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}};

  ck_assert_int_eq(isSquarePiece(&squarePiece), 1);

  ck_assert_int_eq(isSquarePiece(&nonSquarePiece), 0);
}
END_TEST

// Тест для rotateMatrix90
START_TEST(test_rotateMatrix90) {
  int src[4][4] = {{1, 0, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};
  int dest[4][4] = {{0}};

  int expected[4][4] = {{0, 1, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  rotateMatrix90(src, dest);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(dest[i][j], expected[i][j]);
    }
  }

  int src1[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int dest1[4][4] = {{0}};

  int expected1[4][4] = {
      {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  rotateMatrix90(src1, dest1);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(dest1[i][j], expected1[i][j]);
    }
  }
}
END_TEST

START_TEST(test_isRotationValid) {
  int **field = initializeField();
  // int field[ROWS][COLS] = {0};
  // int *fieldPtrs[ROWS];
  // for (int i = 0; i < ROWS; i++) {
  //   fieldPtrs[i] = field[i];
  // }
  Piece *piece = malloc(sizeof(Piece));
  *piece =
      (Piece){5, 5, {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};
  int rotated[4][4] = {
      {0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};  // проверить

  ck_assert(isRotationValid(piece, rotated, 0, 0, field) == true);

  piece->x = 0;
  piece->y = 0;
  ck_assert(isRotationValid(piece, rotated, -1, 0, field) ==
            true);  // проверить

  piece->x = COLS - 2;
  piece->y = ROWS - 2;
  ck_assert(isRotationValid(piece, rotated, 0, 1, field) == false);

  piece->x = 5;
  piece->y = 5;
  for (int i = 0; i <= 9; i++) {
    field[i][7] = 1;
  }
  ck_assert(isRotationValid(piece, rotated, 0, 0, field) == false);

  for (int i = 0; i <= 9; i++) {
    field[i][7] = 0;
  }
  ck_assert(isRotationValid(piece, rotated, 0, 0, field) == true);

  piece->x = 5;
  piece->y = 5;
  rotated[1][1] = 0;
  ck_assert(isRotationValid(piece, rotated, 0, 0, field) == true);
}
END_TEST

// Тест для rotatePiece
START_TEST(test_rotatePiece) {
  int **field = initializeField();

  field[19][5] = 1;

  Piece piece = {5,
                 17,
                 {
                     {1, 1, 1, 0},
                     {0, 1, 0, 0},
                     {0, 0, 0, 0},
                     {0, 0, 0, 0},
                 }};

  int expected[4][4] = {
      {0, 0, 0, 1},
      {0, 0, 1, 1},
      {0, 0, 0, 1},
      {0, 0, 0, 0},
  };

  int expected2[4][4] = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 1, 0},
      {0, 1, 1, 1},
  };

  int expected3[4][4] = {
      {1, 1, 1, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
  };

  rotatePiece(&piece, field);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(piece.shape[i][j], expected[i][j]);
    }
  }

  piece.x = 7;
  piece.y = 5;

  rotatePiece(&piece, field);

  ck_assert_int_le(piece.x, COLS);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(piece.shape[i][j], expected2[i][j]);
    }
  }

  rotatePiece(&piece, field);

  piece.x = 0;
  piece.y = 5;

  rotatePiece(&piece, field);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(piece.shape[i][j], expected3[i][j]);
    }
  }

  ck_assert_int_ge(piece.x, 0);

  Piece squarePiece = {5,
                       5,
                       {
                           {1, 1, 0, 0},
                           {1, 1, 0, 0},
                           {0, 0, 0, 0},
                           {0, 0, 0, 0},
                       }};

  int squareExpected[4][4] = {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
  };

  rotatePiece(&squarePiece, field);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(squarePiece.shape[i][j], squareExpected[i][j]);
    }
  }

  freeField(field);
}
END_TEST

// Тест для applyRotation
START_TEST(test_applyRotation) {
  Piece piece = {
      5, 5, {{0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};
  int rotated[4][4] = {{1, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int expected[4][4] = {{1, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  applyRotation(&piece, rotated, 1, 1);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(rotated[i][j], expected[i][j]);
    }
  }
  ck_assert_int_eq(piece.x, 6);
  ck_assert_int_eq(piece.y, 6);
}
END_TEST

START_TEST(test_fixPartialPiece) {
  int **field = initializeField();
  Piece *piece1 = malloc(sizeof(Piece));
  *piece1 =
      (Piece){5, 6, {{1, 1, 0, 0}, {1, 1, 1, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}}};

  fixPartialPiece(field, piece1);

  ck_assert_int_eq(field[0][5], 1);
  ck_assert_int_eq(field[0][6], 1);
  ck_assert_int_eq(field[0][7], 1);
  ck_assert_int_eq(field[0][8], 1);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (i != 0 || j < 5 || j > 8) {
        ck_assert_int_eq(field[i][j], 0);
      }
    }
  }

  freeField(field);
}
END_TEST

START_TEST(test_isSpaceAvailableForFullFix) {
  int **field = initializeField();

  Piece piece = {5,
                 5,
                 {
                     {1, 1, 0, 0},
                     {0, 1, 1, 0},
                     {0, 0, 0, 0},
                     {0, 0, 0, 0},
                 }};

  ck_assert_int_eq(isSpaceAvailableForFullFix(field, &piece), true);

  field[6][6] = 1;

  ck_assert_int_eq(isSpaceAvailableForFullFix(field, &piece), false);

  piece.x = 0;
  piece.y = 0;

  ck_assert_int_eq(isSpaceAvailableForFullFix(field, &piece), true);

  piece.x = -1;
  piece.y = 0;

  ck_assert_int_eq(isSpaceAvailableForFullFix(field, &piece), false);

  piece.x = 5;
  piece.y = ROWS - 1;

  ck_assert_int_eq(isSpaceAvailableForFullFix(field, &piece), false);
  freeField(field);
}
END_TEST

START_TEST(test_clearFullLines) {
  // Инициализация поля
  int **field = initializeField();

  for (int j = 0; j < COLS; j++) {
    field[18][j] = 1;
  }

  for (int j = 0; j < COLS - 1; j++) {
    field[17][j] = 1;
  }

  for (int j = 0; j < COLS; j++) {
    field[19][j] = 1;
  }

  int cleared = clearFullLines(field);
  ck_assert_int_eq(cleared, 2);

  for (int j = 0; j < COLS - 1; j++) {
    ck_assert_int_eq(field[19][j], 1);
  }
  ck_assert_int_eq(field[19][COLS - 1], 0);

  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < COLS; j++) {
      ck_assert_int_eq(field[i][j], 0);
    }
  }

  freeField(field);
}
END_TEST

START_TEST(test_updateScoreAndLevel) {
  GameInfo_t game = {
      .score = 0,
      .high_score = 500,
      .level = 1,
      .speed = 1,
  };

  updateScoreAndLevel(&game, 1);
  ck_assert_int_eq(game.score, 100);
  ck_assert_int_eq(game.high_score, 500);
  ck_assert_int_eq(game.level, 1);
  ck_assert_int_eq(game.speed, 2);

  updateScoreAndLevel(&game, 5);
  ck_assert_int_eq(game.score, 600);
  ck_assert_int_eq(game.high_score, 600);
  ck_assert_int_eq(game.level, 2);
  ck_assert_int_eq(game.speed, 3);

  game.level = 10;
  game.score = 6000;
  updateScoreAndLevel(&game, 3);
  ck_assert_int_eq(game.score, 6300);
  ck_assert_int_eq(game.high_score, 6300);
  ck_assert_int_eq(game.level, 10);
  ck_assert_int_eq(game.speed, 11);

  game.level = 10;
  game.score = 7000;
  updateScoreAndLevel(&game, 3);
  ck_assert_int_eq(game.score, 7300);
  ck_assert_int_eq(game.high_score, 7300);
  ck_assert_int_eq(game.level, 10);
  ck_assert_int_eq(game.speed, 11);

  game.score = 400;
  game.high_score = 500;
  updateScoreAndLevel(&game, 0);
  ck_assert_int_eq(game.score, 400);
  ck_assert_int_eq(game.high_score, 500);
  ck_assert_int_eq(game.level, 10);
  ck_assert_int_eq(game.speed, 11);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s = suite_create("Tetris");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_can_move_down);
  tcase_add_test(tc_core, test_can_move_left);
  tcase_add_test(tc_core, test_can_move_right);
  tcase_add_test(tc_core, test_save_high_score);
  tcase_add_test(tc_core, test_load_high_score);
  // tcase_add_test(tc_core, test_update_piece_position);
  tcase_add_test(tc_core, test_initialize_game);
  tcase_add_test(tc_core, test_fix_piece);
  tcase_add_test(tc_core, test_isSquarePiece);
  tcase_add_test(tc_core, test_rotateMatrix90);
  tcase_add_test(tc_core, test_isRotationValid);
  tcase_add_test(tc_core, test_rotatePiece);
  tcase_add_test(tc_core, test_applyRotation);
  tcase_add_test(tc_core, test_fixPartialPiece);
  tcase_add_test(tc_core, test_isSpaceAvailableForFullFix);
  tcase_add_test(tc_core, test_clearFullLines);
  tcase_add_test(tc_core, test_updateScoreAndLevel);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
