#include "board.c"
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
    int frames = 0;
    Board *board = Board_new();
    Board_place(board, O);
    system("cls");
    while (true) {
        Board_print(board);
        if (kbhit()) {
            char c = getch();
            switch (c) {
            case 'a':
                board->activePiece.position.x--;
                break;
            case 'd':
                board->activePiece.position.x++;
                break;
            }
        }
        if (frames % 16 == 0) { // lower 16 to increase speed
            Board_update(board);
        }
        frames++;
    }
    return 0;
}