#ifndef PIECE_FIX_H
#define PIECE_FIX_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game_logic.h"

#define ROWS 20
#define COLS 10

void fixPiece(int **field, Piece *piece);
void fixPartialPiece(int **field, Piece *piece);
bool isSpaceAvailableForFullFix(int **field, Piece *piece);

#endif  // PIECE_FIX_H
