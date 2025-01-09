#include "game.h"

int main() {
// Start.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sea Duel");
    SetTargetFPS(60);

    Start();


// Update.
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Update();

        EndDrawing();
    }

// On Window Close
    UnloadGameTextures();
    CloseWindow();
    return 0;
}