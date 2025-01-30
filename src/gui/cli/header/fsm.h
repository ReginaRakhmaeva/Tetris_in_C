#ifndef FSM_H
#define FSM_H
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../brick_game/tetris/header/game_logic.h"
#include "main.h"

void game_loop();
void userInput(UserAction_t action, bool hold);
int processUserInput();
int **getFieldMatrix();
#endif  // FSM_H
