#ifndef FSM_H
#define FSM_H

#include "backend.h"
#include "frontend.h"
#include "tetris.h"
// Управление состояниями игры
void game_loop();
void userInput(UserAction_t action, bool hold);

#endif  // FSM_H
