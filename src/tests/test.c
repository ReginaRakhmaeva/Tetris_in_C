#include <check.h>
#include <stdlib.h>
#define ROWS 20
#define COLS 10
#include <stdio.h>

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
//   // Инициализация игры
//   GameInfo_t game;
//   game.speed = 1.0;

//   // Создание и инициализация поля
//   game.field = (int **)malloc(sizeof(int *) * ROWS);
//   for (int i = 0; i < ROWS; i++) {
//     game.field[i] = (int *)calloc(COLS, sizeof(int));
//   }

//   // Создание фигуры
//   Piece currentPiece = {0};
//   currentPiece.x = 5;
//   currentPiece.y = 0;

//   // Инициализация времени
//   clock_t lastTick = clock();

//   // Тест 1: Фигура может двигаться вниз
//   while ((double)(clock() - lastTick) / CLOCKS_PER_SEC < 0.05 / game.speed) {
//     // Ждем, пока не пройдет время, необходимое для движения
//   }
//   bool result = updatePiecePosition(&currentPiece, &game, &lastTick);
//   ck_assert_int_eq(result, true);
//   ck_assert_int_eq(currentPiece.y, 1);  // Фигура должна сдвинуться вниз

//   // Тест 2: Фигура не может двигаться вниз
//   currentPiece.y = ROWS - 2;  // Перемещаем фигуру на предпоследний ряд
//   for (int i = 0; i < COLS; i++) {
//     game.field[ROWS - 1][i] = 1;  // Заполняем последний ряд
//   }

//   while ((double)(clock() - lastTick) / CLOCKS_PER_SEC < 0.05 / game.speed) {
//     // Ждем снова, чтобы прошло достаточно времени
//   }
//   result = updatePiecePosition(&currentPiece, &game, &lastTick);
//   ck_assert_int_eq(result, false);  // Ожидаем, что функция вернет false
//   ck_assert_int_eq(currentPiece.y, ROWS - 2);  // Фигура остается на месте

//   // Освобождение памяти
//   for (int i = 0; i < ROWS; i++) {
//     free(game.field[i]);
//   }
//   free(game.field);
// }
// END_TEST

Suite *tetris_suite(void) {
  Suite *s = suite_create("Tetris");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_can_move_down);
  tcase_add_test(tc_core, test_can_move_left);
  tcase_add_test(tc_core, test_can_move_right);
  tcase_add_test(tc_core, test_save_high_score);
  tcase_add_test(tc_core, test_load_high_score);
  // tcase_add_test(tc_core, test_update_piece_position);

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
