// #include <stdio.h>
// #include <stdlib.h>
#include <raylib.h>
#include <game1.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// extern int SetConsoleOutputCP(unsigned int codepage);  // Encoding 
// #define CP_UTF8 65001                                  // Encoding

int main()
{
    // SetConsoleOutputCP(CP_UTF8); // Change console encoding to UTF-8

    // Init
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sea-Duel");
    SetTargetFPS(60);

    //
    Ship player1 = InitShip(0, 4);
    Ship player2 = InitShip(9, 4);
    Bullet bullet1 = InitBullet();
    Bullet bullet2 = InitBullet();
    InitObstacles();

    GamePhase phase = MOVEMENT_PHASE;


    // Game loop
    while (!WindowShouldClose()) {
        UpdateGame(&phase, &player1, &player2, &bullet1, &bullet2);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame(player1, player2, bullet1, bullet2);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
