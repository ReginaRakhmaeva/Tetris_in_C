#ifndef PIECE_CREATE_H
#define PIECE_CREATE_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game_logic.h"

#define ROWS 20
#define COLS 10

void spawnNewPiece(Piece **piece);
void initializeNext(GameInfo_t *game, const int shapes[7][4][4]);
void createCurrentPiece(Piece **piece, GameInfo_t *game);
void updateNext(GameInfo_t *game, const int shapes[7][4][4]);

#endif  // PIECE_CREATE_H
