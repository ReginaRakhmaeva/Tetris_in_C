#ifndef PIECE_MOVE_H
#define PIECE_MOVE_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game_logic.h"

#define ROWS 20
#define COLS 10

bool canMoveDown(Piece *piece, int **field);
bool canMoveLeft(Piece *piece, int **field);
bool canMoveRight(Piece *piece, int **field);
bool isSquarePiece(Piece *piece);
void rotateMatrix90(int src[4][4], int dest[4][4]);
bool isRotationValid(Piece *piece, int rotated[4][4], int offsetX, int offsetY,
                     int **field);
void rotatePiece(Piece *piece, int **field);
void applyRotation(Piece *piece, int rotated[4][4], int offsetX, int offsetY);
#endif  // PIECE_MOVE_H
