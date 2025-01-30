#ifndef PIECE_FIX_H
#define PIECE_FIX_H

#include "game_logic.h"

void fixPiece(int **field, Piece *piece);
void fixPartialPiece(int **field, Piece *piece);
bool isSpaceAvailableForFullFix(int **field, Piece *piece);

#endif  // PIECE_FIX_H
