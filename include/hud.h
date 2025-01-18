#ifndef HUD_H
#define HUD_H

#include "game.h"

// Global variables for button textures
extern Texture2D buttonNormal;
extern Texture2D buttonClick;
extern Texture2D buttonHover;

// NEW
typedef struct Button {
    Rectangle bounds;         // Position and size of the button
    Texture *normalTexture;   // Texture for the normal state
    Texture *hoverTexture;    // Texture for the hover state
    Texture *clickTexture;    // Texture for the click state
    const char* text;         // Text to display on the button
    bool isClicked;           // State tracking if the button was clicked
} Button;

void LoadHUDTextures();
void UnloadHUDTextures();

void DrawBackground(Texture *panelTexture);
void DrawScore(Player *players);

Button CreateButton(float x, float y, float width, float height, Texture *normal, Texture *hover, Texture *click, const char* text);
bool UpdateAndDrawButton(Button *button);

#endif // HUD_H
