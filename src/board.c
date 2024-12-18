#include <stdbool.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef enum Piece { O, I, T, L, J, S, Z, PIECES } Piece;

typedef enum Rotation { NORTH, EAST, SOUTH, WEST } Rotation;

typedef bool Shape[4][4];

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Active {
    Piece piece;
    Position position;
    Rotation rotation;
} Active;

typedef struct Board {
    bool map[BOARD_HEIGHT][BOARD_WIDTH];
    Active active;
} Board;

void Piece_getShape(Piece piece, Shape shape) {
    // clang-format off
    Shape shapes[PIECES] = {
        {
            // O
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            // I
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            // T
            {0, 0, 1, 0},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            // L
            {0, 0, 0, 1},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            // J
            {0, 1, 0, 0},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            // S
            {0, 0, 1, 1},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            // Z
            {0, 1, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }
    };
    // clang-format on
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            shape[i][j] = shapes[piece][i][j];
        }
    }
}

/**
 * Send a new active `piece` onto this `board`.
 */
void Board_newActivePiece(Board *board, Piece piece) {
    board->active.piece = piece;
    board->active.position = (Position){.x = 3, .y = -2};
    board->active.rotation = NORTH;
}
/**
 * Lock the current active piece onto this `board`.
 */
void Board_placeActivePiece(Board *board) {
    Shape shape;
    Piece_getShape(board->active.piece, shape);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j]) {
                board->map[i + board->active.position.y]
                          [j + board->active.position.x] = 1;
            }
        }
    }
}

typedef enum TileType { NONE, BOARD, ACTIVE } TileType;

/**
 * Check if a tile exists in the `board` at the (`i`, `j`) coordinates.
 */
TileType Board_tileExists(Board *board, int i, int j) {
    if (board->map[i][j]) {
        return BOARD;
    }
    Shape shape;
    Piece_getShape(board->active.piece, shape);
    for (int i2 = 0; i2 < 4; i2++) {
        for (int j2 = 0; j2 < 4; j2++) {
            if (shape[i2][j2] && i == i2 + board->active.position.y &&
                j == j2 + board->active.position.x) {
                return ACTIVE;
            }
        }
    }
    return NONE;
}

int Active_getLeftBound(Active active) {
    int leftmost = 4;
    Shape shape;
    Piece_getShape(active.piece, shape);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < leftmost; j++) {
            if (shape[i][j] && j < leftmost) {
                leftmost = j;
            }
        }
    }
    return leftmost;
}

int Active_getRightBound(Active active) {
    int rightmost = 0;
    Shape shape;
    Piece_getShape(active.piece, shape);
    for (int i = 0; i < 4; i++) {
        for (int j = rightmost; j < 4; j++) {
            if (shape[i][j] && j > rightmost) {
                rightmost = j;
            }
        }
    }
    return rightmost;
}

int Active_getLowBound(Active active) {
    Shape shape;
    Piece_getShape(active.piece, shape);
    for (int i = 3; i >= 0; i--) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j]) {
                return i;
            }
        }
    }
    return 0;
}

int Active_getHighBound(Active active) {
    Shape shape;
    Piece_getShape(active.piece, shape);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j]) {
                return i;
            }
        }
    }
    return 0;
}

bool Board_moveActiveLeft(Board *board) {
    if (Active_getLeftBound(board->active) + board->active.position.x <= 0) {
        return false;
    }
    board->active.position.x--;
    return true;
}

bool Board_moveActiveRight(Board *board) {
    if (Active_getRightBound(board->active) + board->active.position.x >=
        BOARD_WIDTH - 1) {
        return false;
    }
    board->active.position.x++;
    return true;
}

bool Board_moveActiveDown(Board *board) {
    int posY = board->active.position.y;
    int posX = board->active.position.x;
    int lowBound = Active_getLowBound(board->active);
    int leftBound = Active_getLeftBound(board->active);
    int rightBound = Active_getRightBound(board->active);
    if (lowBound + posY >= BOARD_HEIGHT - 1) {
        return false;
    }
    Shape shape;
    Piece_getShape(board->active.piece, shape);
    for (int i = leftBound; i <= rightBound; i++) {
        if (shape[lowBound][i] && board->map[lowBound + 1 + posY][i + posX]) {
            return false;
        }
    }
    board->active.position.y++;
    return true;
}