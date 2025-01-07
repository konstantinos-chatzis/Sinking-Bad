#ifndef FIRING_COMMANDS_H
#define FIRING_COMMANDS_H

#include "game.h"

void FiringCommandsInput(Player (*players)[2], int* currentPlayerIndex);
void FiringCommandsDrawing(Player (*players)[2], int* currentPlayerIndex);

#endif // FIRING_COMMANDS_H