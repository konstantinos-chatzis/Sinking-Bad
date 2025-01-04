// #define RAYGUI_IMPLEMENTATION
#include <game.h>
#include <phase_movement_commands.h> // TODO: Move load texture functions to game.c

int main() {
// Start.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sea Duel");
    SetTargetFPS(60);

    LoadGameTextures();
    LoadSpeedSelectioTextures();

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

