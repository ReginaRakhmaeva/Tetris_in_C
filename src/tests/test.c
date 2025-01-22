#include <check.h>
#include <stdlib.h>
#define ROWS 20
#define COLS 10
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

Suite *tetris_suite(void) {
  Suite *s = suite_create("Tetris");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_can_move_down);
  tcase_add_test(tc_core, test_can_move_left);
  tcase_add_test(tc_core, test_can_move_right);

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
