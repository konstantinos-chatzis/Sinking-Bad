#include <raylib.h>
#include <raymath.h>
#include <game.h>

int main() {
// Start.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sea Duel");
    SetTargetFPS(60);



    LoadGameTextures();

// Update.
    while (!WindowShouldClose()) {
        UpdateGameInput();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        UpdateGameDraw();
        EndDrawing();
    }

    UnloadGameTextures();
    CloseWindow();
    return 0;
}

