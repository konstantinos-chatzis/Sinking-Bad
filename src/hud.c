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

bool DrawButton(float posX, float posY, float widthFactor, float heightFactor, Texture *buttonNormal, Texture *buttonHover, Texture *buttonClick, const char* text) {
    // Draw confirmation button
    static bool buttonClicked = false;
    Rectangle buttonBounds = {
        posX,
        posY,
        buttonNormal->width * widthFactor,
        buttonNormal->height * heightFactor
    };
    Vector2 mousePosition = GetMousePosition();

    if (CheckCollisionPointRec(mousePosition, buttonBounds)) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            DrawTexturePro(
                *buttonClick,
                (Rectangle){0, 0, buttonClick->width, buttonClick->height},
                buttonBounds,
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
            buttonClicked = true; 
        } else if (buttonClicked && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            // Button action is triggered only when the mouse is released over the button
            return true;

        } else {
            DrawTexturePro(
                *buttonHover,
                (Rectangle){0, 0, buttonHover->width, buttonHover->height},
                buttonBounds,
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
        }
    } else {
        DrawTexturePro(
            *buttonNormal,
            (Rectangle){0, 0, buttonNormal->width, buttonNormal->height},
            buttonBounds,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            buttonClicked = false; // Reset the button state if the mouse is released outside
        }
    }
    DrawText(text, buttonBounds.x + 17, buttonBounds.y + 18, 30, DARKGRAY);
    return false;
}