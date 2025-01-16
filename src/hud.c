#include "hud.h"

void DrawBackground(Texture *backgroundTexture) {
    float scale = 11.0f;

    DrawTextureEx(
        *backgroundTexture,
        (Vector2){0, 0},
        0.0f,
        scale,
        WHITE
    );
}

void DrawTimer() {
    
}

void DrawScore(Player *players) {
    DrawText("Score:", 1200, 40, 50, RAYWHITE);
    DrawText(TextFormat("%d", players[0].score), 1380, 40, 50, BLUE);
    DrawText("|", 1420, 40, 50, LIGHTGRAY);
    DrawText(TextFormat("%d", players[1].score), 1440, 40, 50, RED);
}