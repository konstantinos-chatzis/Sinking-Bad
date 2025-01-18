#include "file_handler.h"

void SaveGameFile(GamePhase *currenPhase, int *currentPlayerIndex, Player *players, float *timer, float *originalShipRotation1, float *originalShipRotation2) {
    FILE *file = fopen("saves/gamesave.dat", "wb");
    if (!file) {
        printf("Error: Could not save game state\n");
        return;
    }

    fwrite(currenPhase, sizeof(GamePhase), 1, file);
    fwrite(currentPlayerIndex, sizeof(int), 1, file);
    fwrite(timer, sizeof(float), 1, file);
    fwrite(originalShipRotation1, sizeof(float), 1, file);
    fwrite(originalShipRotation2, sizeof(float), 1, file);
    fwrite(players, sizeof(Player), 2, file);
    

    fclose(file);
    printf("Game state saved successfully to savegame.dat\n");
}

void LoadGameFile(GamePhase *currentPhase, int *currentPlayerIndex, Player *players, float *timer, float *originalShipRotation1, float *originalShipRotation2) {
    FILE *file = fopen("saves/gamesave.dat", "rb");
    if (!file) {
        printf("Error: Could not load game state from gamesave.dat\n");
        return;
    }

    fread(currentPhase, sizeof(GamePhase), 1, file);
    fread(currentPlayerIndex, sizeof(int), 1, file);
    fread(timer, sizeof(float), 1, file);
    fread(originalShipRotation1, sizeof(float), 1, file);
    fread(originalShipRotation2, sizeof(float), 1, file);
    fread(players, sizeof(Player), 2, file);

    fclose(file);
    printf("Game state loaded successfully from gamesave.dat\n");
}