#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum TetriminoType {
    O,
    I,
    T,
    L,
    J,
    S,
    Z,
    LEN_TERTIMINO_TYPES
} TetriminoType;

typedef enum Rotation { North, East, South, West, LEN_ROTATIONS } Rotation;

typedef bool Shape[4][4];

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Piece {
    TetriminoType type;
    Rotation rotation;
    Position position;
} Piece;

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef struct Board {
    bool map[BOARD_HEIGHT][BOARD_WIDTH];
    Piece activePiece;
} Board;
