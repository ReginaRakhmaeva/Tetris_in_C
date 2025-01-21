#include "tetris.h"

#include "../../gui/cli/frontend.h"
#include "backend.h"
#include "fsm.h"
#include "tetris.h"

GameInfo_t *getGameInfo() {
  static GameInfo_t game;
  return &game;
}
Piece *getCurrentPiece() {
  static Piece *piece = NULL;
  if (!piece) {
    spawnNewPiece(&piece);  // Инициализация новой фигуры
  }
  return piece;
}
