#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "game.h"
#include "hud.h"

void LoadPauseTextures();
void UnloadPauseTextures();

void DrawPauseMenu(Button *saveButton, Button *pauseMenuBackToTitleButton);

#endif // PAUSE_MENU_H