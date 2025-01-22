#ifndef FRONTEND_H
#define FRONTEND_H
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/tetris/game_logic.h"
#define ROWS 20
#define COLS 10
// Функции отображения
void renderStartScreen();
bool showStartScreen();

void drawField(GameInfo_t *game);
void drawStaticField(int **field);
void drawPiece(Piece *piece);
void drawGameInfo(GameInfo_t *game);
void clearField();
void showGameOverScreen(GameInfo_t *game);
void initNcurses();
void cleanupNcurses(GameInfo_t *game);
int processUserInput();
void cleanupNcursesstart();

void game_loop();

void userInput(UserAction_t action, bool hold);
bool handleStartScreenInput();
bool handleGameOverInput();
GameInfo_t updateCurrentState();
void handleGameOver(GameInfo_t *game, Piece *currentPiece);

#endif  // FRONTEND_H
