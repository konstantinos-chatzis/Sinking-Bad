#ifndef PROJECTILE_MOVEMENT_H
#define PROJECTILE_MOVEMENT_H

#include "game.h"

void FiringCommandsInput(Player *players, int* currentPlayerIndex);
void FiringCommandsDrawing(Player *players, int* currentPlayerIndex);
void UpdateProjectileMovement(Player *players);

#endif // PROJECTILE_MOVEMENT_H