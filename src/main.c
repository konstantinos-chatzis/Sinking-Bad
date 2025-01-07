#include "game.h"
#include "movement_commands.h" // TODO: Move load texture functions to game.c

int main() {
// Start.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sea Duel");
    SetTargetFPS(60);

    LoadGameTextures();
    LoadSpeedSelectionTextures();
    InitHitboxes();

// Update.
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Update();

        EndDrawing();
    }

// On Window Close
    UnloadGameTextures();
    UnloadSpeedSelectionTextures();
    UnloadSpeedSelectionTextures();
    CloseWindow();
    return 0;
}

