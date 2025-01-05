#include <game.h>
// Game modules
#include <phase_ship_deployment.h>
#include <phase_movement_commands.h>
#include <phase_movement_half.h>


Player players[2] = {
    {0, {{0, 0}, 0.0f, 1.0f, BLUE}, BLUE, false, false, false},
    {1, {{0, 0}, 0.0f, 1.0f, RED}, RED, false, false, false}
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
            printf("%.1f\n", players[0].ship.speed);
            printf("%.1f\n", players[1].ship.speed);
            ShipDeploymentInput(&players, &currentPlayerIndex);
            ShipDeploymentDrawing(&players, &currentPlayerIndex);
            if(players[0].hasDeployed && players[1].hasDeployed) currentPhase++;
            break;
        case MOVEMENT_COMMANDS:
            ShipDirectionInput(&players, &currentPlayerIndex);
            ShipDirectionDrawing(&players, &currentPlayerIndex);
            if (players[currentPlayerIndex].hasSelectedDirection) ShipSpeedInputDrawing(&players, &currentPlayerIndex, 1.0f, 5.0f);
            if (players[0].hasSelectedSpeed && players[1].hasSelectedSpeed) currentPhase++;
            break;
        case MOVEMENT_HALF:
            UpdateMovementHalfPhase(&players, GetFrameTime());
            DrawMovementHalfPhase(&players);

            if (IsMovementHalfPhaseComplete()) {
                // currentPhase++;  // Move to the next phase
                printf("Movement half phase complete\n");
            }
            break;
        case FIRING_COMMANDS:
            //
            break;
        case MOVEMENT_REST:
            //
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
