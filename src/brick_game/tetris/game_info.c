#include "game_logic.h"
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