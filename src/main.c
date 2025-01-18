#include "game.h"

int main() {
// Start.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sinking Bad");
    SetWindowIcon(LoadImage("assets/sprites/icon.png"));
    SetTargetFPS(60);
    SetExitKey(0);

    Start();


// Update.
    while (!WindowShouldClose()) {
        BeginDrawing();
        

        Update();

        EndDrawing();
    }

// On Window Close
    UnloadGameTextures();
    CloseWindow();
    return 0;
}