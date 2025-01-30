#ifndef TETRIS_INTERFACE_H
#define TETRIS_INTERFACE_H
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../brick_game/tetris/header/game_logic.h"
#include "main.h"

// Функции отображения
void renderStartScreen();

void drawField(GameInfo_t *game);
void drawStaticField(int **field);
void drawPiece(Piece *piece);
void drawGameInfo(GameInfo_t *game);
void clearField();
void displayGameOverScreen(GameInfo_t *game);
void handleGameOver(GameInfo_t *game, Piece *currentPiece);

int processUserInput();
#endif  // FRONTEND_H
