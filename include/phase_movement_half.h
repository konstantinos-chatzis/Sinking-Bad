#ifndef PHASE_MOVEMENT_HALF_H
#define PHASE_MOVEMENT_HALF_H

#include <game.h>

void InitMovementHalfPhase();
void UpdateMovementHalfPhase(Player (*players)[2], float deltaTime);
void DrawMovementHalfPhase(Player (*players)[2]);
bool IsMovementHalfPhaseInitialized();
bool IsMovementHalfPhaseComplete();

#endif
