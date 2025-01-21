#ifndef FRONTEND_H
#define FRONTEND_H

#include "../../brick_game/tetris/backend.h"
#include "../../brick_game/tetris/fsm.h"
#include "../../brick_game/tetris/tetris.h"
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

#endif  // FRONTEND_H
