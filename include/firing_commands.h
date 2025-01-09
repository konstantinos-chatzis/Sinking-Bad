#ifndef FIRING_COMMANDS_H
#define FIRING_COMMANDS_H

#include "game.h"

void FiringCommandsInput(Player *players, int* currentPlayerIndex);
void FiringCommandsDrawing(Player *players, int* currentPlayerIndex);
void UpdateBulletMovement(Player *players);

#endif // FIRING_COMMANDS_H