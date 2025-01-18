#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "game.h"
#include <stdio.h>

void SaveGameFile(GamePhase *currenPhase, int *currentPlayerIndex, Player *players, float *timer, float *originalShipRotation1, float *originalShipRotation2);
void LoadGameFile(GamePhase *currentPhase, int *currentPlayerIndex, Player *players, float *timer, float *originalShipRotation1, float *originalShipRotation2);

#endif // FILE_HANDLER_H