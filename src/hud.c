#include "hud.h"

Texture2D buttonNormal;
Texture2D buttonClick;
Texture2D buttonHover;

void LoadHUDTextures() {
    // Button textures
    buttonNormal = LoadTexture("assets/sprites/button_normal.png");
    buttonHover = LoadTexture("assets/sprites/button_hover.png");
    buttonClick = LoadTexture("assets/sprites/button_clicked.png");
}

void UnloadHUDTextures() {
    UnloadTexture(buttonNormal);
    UnloadTexture(buttonHover);
    UnloadTexture(buttonClick);
}

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

void DrawScore(Player *players) {
    DrawText("Score:", 1200, 40, 50, RAYWHITE);
    DrawText(TextFormat("%d", players[0].score), 1380, 40, 50, BLUE);
    DrawText("|", 1420, 40, 50, LIGHTGRAY);
    DrawText(TextFormat("%d", players[1].score), 1440, 40, 50, RED);
}

Button CreateButton(float x, float y, float width, float height, 
                    Texture *normal, Texture *hover, Texture *click, const char* text) {
    return (Button) {
        .bounds = (Rectangle){x, y, width, height},
        .normalTexture = normal,
        .hoverTexture = hover,
        .clickTexture = click,
        .text = text,
        .isClicked = false
    };
}

bool UpdateAndDrawButton(Button *button) {
    Vector2 mousePosition = GetMousePosition();
    bool isMouseOver = CheckCollisionPointRec(mousePosition, button->bounds);

    if (isMouseOver) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            DrawTexturePro(
                *button->clickTexture,
                (Rectangle){0, 0, button->clickTexture->width, button->clickTexture->height},
                button->bounds,
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
            button->isClicked = true;
        } else if (button->isClicked && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            button->isClicked = false; // Reset click state
            return true; // Action is triggered
        } else {
            DrawTexturePro(
                *button->hoverTexture,
                (Rectangle){0, 0, button->hoverTexture->width, button->hoverTexture->height},
                button->bounds,
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
        }
    } else {
        DrawTexturePro(
            *button->normalTexture,
            (Rectangle){0, 0, button->normalTexture->width, button->normalTexture->height},
            button->bounds,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );

        // Reset click state if mouse leaves button
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            button->isClicked = false;
        }
    }

    int textWidth = MeasureText(button->text, 30);
    float textX = button->bounds.x + (button->bounds.width - textWidth) / 2;
    float textY = button->bounds.y + (button->bounds.height - 30) / 2;
    DrawText(button->text, textX, textY, 30, DARKGRAY);

    return false; // No action triggered
}