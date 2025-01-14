#ifndef FRONTEND_H
#define FRONTEND_H

#include "tetris.h"

// Функции отображения
void renderStartScreen();
bool handleStartScreenInput();
bool showStartScreen();
void drawField(GameInfo_t *game);
void drawStaticField(int **field);
void drawPiece(Piece *piece);
void drawGameInfo(GameInfo_t *game);
void clearField();
void showGameOverScreen(GameInfo_t *game);

#endif  // FRONTEND_H
