#ifndef MAIN_H
#define MAIN_H
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../brick_game/tetris/game_logic.h"
#include "fsm.h"
#include "tetris_interface.h"
#define WIN_INIT(size) initNcurses()

GameInfo_t updateCurrentState();
void initNcurses();
void cleanupNcurses(GameInfo_t *game);
void cleanupNcursesstart();
void showGameOverScreen(GameInfo_t *game);
bool handleStartScreenInput();
bool showStartScreen();

#endif  // MAIN_H
