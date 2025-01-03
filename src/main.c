#define RAYGUI_IMPLEMENTATION
#include <game.h>
#include <phase_movement_commands.h>

int main() {
// Start.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sea Duel");
    SetTargetFPS(60);

    LoadGameTextures();
    LoadSliderTextures();

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

