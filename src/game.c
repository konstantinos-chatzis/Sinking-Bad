#include "game.h"
// Game modules
#include "ship_deployment.h"
#include "movement_commands.h"
#include "movement_half.h"


Player players[2] = {
    {0, {{0, 0}, 0.0f, 1.0f, 0.0f, BLUE}, BLUE, false, false, false},
    {1, {{0, 0}, 0.0f, 1.0f, 0.0f, RED}, RED, false, false, false}
};

int currentPlayerIndex = 0; // 0 for player 1 (BLUE), 1 for player 2 (RED)
GamePhase currentPhase = SHIP_DEPLOYMENT;

Texture2D LoadShipTexture(Color color) {
    char* colorName;

    // Determine the color name based on the Color
    if (color.r == BLUE.r && color.g == BLUE.g && color.b == BLUE.b && color.a == BLUE.a) {
        colorName = "blue";
    } else if (color.r == RED.r && color.g == RED.g && color.b == RED.b && color.a == RED.a) {
        colorName = "red";
    } else {
        colorName = "default";
    }

    // Construct the file path dynamically
    char filePath[100];
    snprintf(filePath, sizeof(filePath), "assets/sprites/ship_%s.png", colorName);

    // Load and return the texture
    return LoadTexture(filePath);
}

void LoadGameTextures(){
    for(int i = 0; i < 2;i++){
        players[i].ship.texture = LoadShipTexture(players[i].ship.color);
    }
}

void UnloadGameTextures(){
    for(int i = 0; i < 2;i++){
        UnloadTexture(players[i].ship.texture);
    }
}

void HandleGamePhases(){
    switch (currentPhase) {
        case TITLE_SCREEN:
            //
            break;
        case SHIP_DEPLOYMENT:
            ShipDeploymentInput(&players, &currentPlayerIndex);
            ShipDeploymentDrawing(&players, &currentPlayerIndex);
            if(players[0].hasDeployed && players[1].hasDeployed) currentPhase++;
            break;
        case MOVEMENT_COMMANDS:
            ShipDirectionInput(&players, &currentPlayerIndex);
            ShipDirectionDrawing(&players, &currentPlayerIndex);
            if (players[currentPlayerIndex].hasSelectedDirection) ShipSpeedInputDrawing(&players, &currentPlayerIndex, 1.0f, 10.0f);
            if (players[0].hasSelectedSpeed && players[1].hasSelectedSpeed) currentPhase++;
            break;
        case MOVEMENT_HALF:
            UpdateMovementHalfPhase(&players, GetFrameTime());
            DrawMovementHalfPhase(&players);

            // if (IsMovementPhaseHalfComplete()) {
            //     currentPhase = FIRING_COMMANDS;
            // }
            break;
        case FIRING_COMMANDS:
            // Condition to move to next phase
                currentPhase = MOVEMENT_HALF;
            break;
        case MOVEMENT_REST:
            if (IsMovementPhaseComplete()) {
                currentPhase++;
            }
            break;
        case FINAL_EXCECUTION:
            //
            break;
        case RESULTS:
            //
            break;
    }
}

void Update(){
    HandleGamePhases();

    
}
