#include "board.h"

bool within(int low, int x, int high) {
    return low <= x && x <= high;
}

void getPieceShape(Piece piece, Shape out) {
    Shape shapes[LEN_TERTIMINO_TYPES][LEN_ROTATIONS] = {0};
    shapes[O][North][0][2] = 1;
    shapes[O][North][0][3] = 1;
    shapes[O][North][1][2] = 1;
    shapes[O][North][1][3] = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out[i][j] = shapes[piece.type][piece.rotation][i][j];
        }
    }
}

Board *Board_new() {
    return (Board *)calloc(1, sizeof(Board));
}

void Board_place(Board *this, TetriminoType t) {
    this->activePiece = (Piece){.type = t,
                                .rotation = North,
                                .position = (Position){.x = 3, .y = 0}};
}

void Board_update(Board *this) {
    this->activePiece.position.y++;
}

void locate(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void Board_print(Board *this) {
    Shape shape;
    Position position = this->activePiece.position;
    getPieceShape(this->activePiece, shape);
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            locate(j * 2, i);
            if (this->map[i][j] || (within(position.y, i, position.y + 3) &&
                                    within(position.x, j, position.x + 3) &&
                                    shape[i - position.y][j - position.x])) {
                printf("[]");
            } else {
                printf(" .");
            }
        }
        printf("\n");
    }
}