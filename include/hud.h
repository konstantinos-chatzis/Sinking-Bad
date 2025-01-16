#ifndef HUD_H
#define HUD_H

#include "game.h"

// typedef struct Button { // Behaves like an "Object"
//     float posX;
//     float posY;
//     float widthFactor;
//     float heightFactor;
//     Texture buttonNormal;
//     Texture buttonHover;
//     Texture buttonClick;

//     // Function pointer
//     void 
// } Button;

void DrawBackground(Texture *panelTexture);
void DrawTimer();
void DrawScore(Player *players);

bool DrawButton(float posX, float posY, float widthFactor, float heightFactor, Texture *buttonNormal, Texture *buttonHover, Texture *buttonClick, const char* text);

#endif // HUD_H
