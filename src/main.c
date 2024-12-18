#include "../lib/raylib.h"
#include "board.c"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Game
    Board *board = calloc(1, sizeof(Board));
    Board_newActivePiece(board, O);

    // GUI
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int tileSize = 32;
    const int boardWidth = BOARD_WIDTH;
    const int boardHeight = BOARD_HEIGHT;
    const int boardOffsetX = screenWidth / 2 - boardWidth * tileSize / 2;
    const int boardOffsetY = screenHeight / 2 - boardHeight * tileSize / 2;
    int screen = 0;
    int frames = 0;
    InitWindow(screenWidth, screenHeight, "Brick Game");
    SetTargetFPS(60);

    // Music
    InitAudioDevice();
    Music music = LoadMusicStream("media/bgm.ogg");
    PlayMusicStream(music);
    while (!WindowShouldClose()) {
        // GUI
        BeginDrawing();
        ClearBackground(BLACK);

        if (screen == 0) {
            DrawText("Press ENTER to start", 0, 0, 32, WHITE);
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER)) {
                screen = 1;
            }
            continue;
        }
        // Board
        DrawRectangle(boardOffsetX, boardOffsetY, tileSize * boardWidth,
                      tileSize * boardHeight, DARKGRAY);
        // Tiles
        for (int i = 0; i < boardHeight; i++) {
            for (int j = 0; j < boardWidth; j++) {
                TileType tileExists = Board_tileExists(board, i, j);
                if (tileExists == BOARD) {
                    DrawRectangle(boardOffsetX + tileSize * j,
                                  boardOffsetY + tileSize * i, tileSize,
                                  tileSize, RED);
                } else if (tileExists == ACTIVE) {
                    DrawRectangle(boardOffsetX + tileSize * j,
                                  boardOffsetY + tileSize * i, tileSize,
                                  tileSize, BLUE);
                }
            }
        }
        // Grid
        for (int i = 0; i <= boardHeight; i++) { // Horizontal lines
            DrawLine(boardOffsetX, boardOffsetY + tileSize * i,
                     boardOffsetX + boardWidth * tileSize,
                     boardOffsetY + tileSize * i, WHITE);
        }
        for (int i = 0; i <= boardWidth; i++) { // Vertical lines
            DrawLine(boardOffsetX + tileSize * i, boardOffsetY,
                     boardOffsetX + tileSize * i,
                     boardOffsetY + tileSize * boardHeight, WHITE);
        }
        // Text
        const int textLeftX = boardOffsetX / 10;
        const int textRightX = screenWidth - boardOffsetX + boardOffsetX / 10;
        const int textUnitY = boardHeight / 3;
        const int textSize = 32;
        DrawText("Next", textLeftX, boardOffsetY, textSize, WHITE);
        DrawText("Hold", textRightX, boardOffsetY, textSize, WHITE);
        DrawText("Score", textLeftX, boardOffsetY + tileSize * textUnitY,
                 textSize, WHITE);
        DrawText("Rank", textRightX, boardOffsetY + tileSize * textUnitY,
                 textSize, WHITE);
        DrawText("Level", textLeftX, boardOffsetY + tileSize * 2 * textUnitY,
                 textSize, WHITE);
        DrawText("Time", textRightX, boardOffsetY + tileSize * 2 * textUnitY,
                 textSize, WHITE);
        EndDrawing();

        // Audio
        UpdateMusicStream(music);

        // Game
        frames++;
        if (frames % 10 == 0) {
            if (!Board_moveActiveDown(board)) {
                Board_placeActivePiece(board);
                Board_newActivePiece(board, rand() % PIECES);
            }
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressedRepeat(KEY_A)) {
            Board_moveActiveLeft(board);
        }
        if (IsKeyPressed(KEY_D) || IsKeyPressedRepeat(KEY_D)) {
            Board_moveActiveRight(board);
        }
    }
    StopMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}