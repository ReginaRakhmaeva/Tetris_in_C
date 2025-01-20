#ifndef FRONTEND_H
#define FRONTEND_H

#include "backend.h"
#include "tetris.h"
// Функции отображения
void renderStartScreen();
bool showStartScreen();
void drawField(GameInfo_t *game);
void drawStaticField(int **field);
void drawPiece(Piece *piece);
void drawGameInfo(GameInfo_t *game);
void clearField();
void showGameOverScreen(GameInfo_t *game);

#endif  // FRONTEND_H
