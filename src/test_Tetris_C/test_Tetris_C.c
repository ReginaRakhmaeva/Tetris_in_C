#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/backend.h"
// #include "../brick_game/tetris/fsm.h"
#include "../brick_game/tetris/tetris.h"
START_TEST(test_can_move_down) {
  // GameInfo_t *game = getGameInfo();
  Piece *piece = getCurrentPiece();

  // Инициализация поля
  int **field = (int **)malloc(sizeof(int *) * ROWS);
  for (int i = 0; i < ROWS; i++) {
    field[i] = (int *)calloc(COLS, sizeof(int));
  }

  // Установим фигуру на поле
  piece->x = 5;
  piece->y = 5;

  // Тестируем, что фигура может двигаться вниз
  ck_assert_int_eq(canMoveDown(piece, field), true);

  // Заполним строку, чтобы фигура не могла двигаться вниз
  field[6][5] = 1;

  // Тестируем, что фигура не может двигаться вниз
  ck_assert_int_eq(canMoveDown(piece, field), false);

  // Освобождаем память
  for (int i = 0; i < ROWS; i++) {
    free(field[i]);
  }
  free(field);
}
END_TEST

// START_TEST(test_spawn_new_piece) {
//   Piece *piece = NULL;
//   spawnNewPiece(&piece);

//   // Проверяем, что фигура была создана
//   ck_assert_ptr_nonnull(piece);

//   // Проверяем, что фигура имеет правильные размеры
//   bool valid_shape = false;
//   for (int i = 0; i < 4; i++) {
//     for (int j = 0; j < 4; j++) {
//       if (piece->shape[i][j]) {
//         valid_shape = true;
//       }
//     }
//   }
//   ck_assert_int_eq(valid_shape, true);

//   free(piece);
// }
// END_TEST

// START_TEST(test_rotate_piece) {
//   // GameInfo_t *game = getGameInfo();
//   Piece *piece = getCurrentPiece();

//   // Инициализация поля
//   int **field = (int **)malloc(sizeof(int *) * ROWS);
//   for (int i = 0; i < ROWS; i++) {
//     field[i] = (int *)calloc(COLS, sizeof(int));
//   }

//   // Инициализируем фигуру (например, квадрат)
//   piece->shape[0][0] = 1;
//   piece->shape[0][1] = 1;
//   piece->shape[1][0] = 1;
//   piece->shape[1][1] = 1;

//   // Тестируем поворот
//   rotatePiece(piece, field);

//   // После поворота квадрат не должен измениться
//   ck_assert_int_eq(piece->shape[0][0], 1);
//   ck_assert_int_eq(piece->shape[0][1], 1);
//   ck_assert_int_eq(piece->shape[1][0], 1);
//   ck_assert_int_eq(piece->shape[1][1], 1);

//   // Освобождаем память
//   for (int i = 0; i < ROWS; i++) {
//     free(field[i]);
//   }
//   free(field);
// }
// END_TEST

// START_TEST(test_clear_full_lines) {
//   // GameInfo_t *game = getGameInfo();

//   // Инициализация поля
//   int **field = (int **)malloc(sizeof(int *) * ROWS);
//   for (int i = 0; i < ROWS; i++) {
//     field[i] = (int *)calloc(COLS, sizeof(int));
//   }

//   // Заполним одну строку
//   for (int j = 0; j < COLS; j++) {
//     field[0][j] = 1;
//   }

//   // Тестируем очистку строк
//   int lines_cleared = clearFullLines(field);
//   ck_assert_int_eq(lines_cleared, 1);

//   // Проверим, что строка очищена
//   for (int j = 0; j < COLS; j++) {
//     ck_assert_int_eq(field[0][j], 0);
//   }

//   // Освобождаем память
//   for (int i = 0; i < ROWS; i++) {
//     free(field[i]);
//   }
//   free(field);
// }
// END_TEST

Suite *tetris_suite(void) {
  Suite *s = suite_create("Tetris");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_can_move_down);
  // tcase_add_test(tc_core, test_clear_full_lines);
  // tcase_add_test(tc_core, test_rotate_piece);
  // tcase_add_test(tc_core, test_spawn_new_piece);

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
