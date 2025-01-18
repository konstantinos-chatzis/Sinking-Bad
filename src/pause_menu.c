#include "pause_menu.h"

Texture pauseOverlay;

void LoadPauseTextures() {
    pauseOverlay = LoadTexture("assets/sprites/pause_overlay.png");
}

void UnloadPauseTextures() {
    UnloadTexture(pauseOverlay);
}

void DrawPauseMenu(Button *saveButton, Button *pauseMenuBackToTitleButton) {
    DrawTextureEx(pauseOverlay, (Vector2){-1, -1}, 0, 1, DARKGRAY);
    DrawText("Pause", 774, 60, 70, WHITE);

    if(UpdateAndDrawButton(saveButton)) {
        // Save
        printf("Save\n");
        isGamePaused = false;
    }
    if (UpdateAndDrawButton(pauseMenuBackToTitleButton)) {
        // Back to Title
        ActionBackToTitle();
        isGamePaused = false;
    }

}