#ifndef PHASE_MOVEMENT_COMMANDS_H
#define PHASE_MOVEMENT_COMMANDS_H

#include <game.h>

// Constants
#define SLIDER_WIDTH 40
#define SLIDER_HEIGHT 400
#define SLIDER_X 100
#define SLIDER_Y 200
#define BUTTON_SCALE 5

// Direction Selection Functions
void ShipDirectionInput(Player (*players)[2], int* currentPlayerIndex);
void ShipDirectionDrawing(Player (*players)[2], int* currentPlayerIndex);

// Speed Selection Functions
void ShipSpeedInputDrawing(Player (*players)[2], int* currentPlayerIndex, float minSpeed, float maxSpeed);

// Texture Loading Functions
void LoadSpeedSelectionTextures();
void UnloadSpeedSelectionTextures();

#endif