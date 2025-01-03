#ifndef PHASE_MOVEMENT_COMMANDS_H
#define PHASE_MOVEMENT_COMMANDS_H

#include <game.h>

void ShipDirectionInput(Player (*players)[2], int* currentPlayerIndex);
void ShipDirectionDrawing(Player (*players)[2], int* currentPlayerIndex);

void ShipSpeedInputDrawing(Player (*players)[2], int* currentPlayerIndex);

void LoadSliderTextures();

#endif