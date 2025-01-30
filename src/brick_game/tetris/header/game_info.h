#ifndef GAME_INFO_H
#define GAME_INFO_H
#include <stdio.h>

#include "objects.h"
#include "piece_create.h"

GameInfo_t *getGameInfo();
Piece *getCurrentPiece();

#endif  // GAME_INFO_H