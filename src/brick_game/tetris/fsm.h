#ifndef FSM_H
#define FSM_H

#include "../../gui/cli/frontend.h"
#include "backend.h"
#include "tetris.h"
// Управление состояниями игры
void game_loop();
void userInput(UserAction_t action, bool hold);
// Явная функция для состояния поля
int **getFieldMatrix();

#endif  // FSM_H
