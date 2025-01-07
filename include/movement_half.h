#ifndef MOVEMENT_HALF_H
#define MOVEMENT_HALF_H

#include "game.h"

void UpdateMovementHalfPhase(Player (*players)[2], float deltaTime);
bool IsMovementPhaseHalfComplete();
bool IsMovementPhaseComplete();

#endif
