#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "game.h"

void UpdateMovement(Player *players, float deltaTime);
bool IsMovementPhaseHalfComplete();
bool IsMovementPhaseComplete();

#endif // MOVEMENT_H
