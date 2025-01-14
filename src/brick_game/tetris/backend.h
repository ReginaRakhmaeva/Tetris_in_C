#ifndef BACKEND_H
#define BACKEND_H

#include <time.h>

#include "frontend.h"
#include "tetris.h"
//  Функции обработки логики игры
void spawnNewPiece(Piece **piece);
bool canMoveDown(Piece *piece, int **field);
bool canMoveLeft(Piece *piece, int **field);
bool canMoveRight(Piece *piece, int **field);
bool isSpaceAvailableForFullFix(int **field, Piece *piece);
void fixPiece(int **field, Piece *piece);
void fixPartialPiece(int **field, Piece *piece);
int clearFullLines(int **field);
GameInfo_t updateCurrentState();
void rotateMatrix90(int src[4][4], int dest[4][4]);
bool isRotationValid(Piece *piece, int rotated[4][4], int offsetX, int offsetY,
                     int **field);
void applyRotation(Piece *piece, int rotated[4][4], int offsetX, int offsetY);
void rotatePiece(Piece *piece, int **field);
bool isSquarePiece(Piece *piece);

#endif  // BACKEND_H
