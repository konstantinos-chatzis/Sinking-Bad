#include <raylib.h>
#include <raymath.h>
#include <game.h>

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sea Duel");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateGameInput();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        UpdateGameDraw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

