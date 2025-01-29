#ifndef FSM_H
#define FSM_H
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/tetris/game_logic.h"
#include "main.h"
#include "tetris_interface.h"
#define GET_USER_INPUT getch()

void game_loop();

void userInput(UserAction_t action, bool hold);
int processUserInput();
#endif  // FSM_H
