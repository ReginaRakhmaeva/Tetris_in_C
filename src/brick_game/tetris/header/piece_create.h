#ifndef PIECE_CREATE_H
#define PIECE_CREATE_H

#include "game_logic.h"

void spawnNewPiece(Piece **piece);
void initializeNext(GameInfo_t *game, const int shapes[7][4][4]);
void createCurrentPiece(Piece **piece, GameInfo_t *game);
void updateNext(GameInfo_t *game, const int shapes[7][4][4]);

#endif  // PIECE_CREATE_H
