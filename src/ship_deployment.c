#include "ship_deployment.h"

bool wasMouseButtonPressed = false;
// 
void ShipDeploymentInput(Player *players, int* currentPlayerIndex){ // This function goes in UpdateGameInput() in game.c
    Player* currentPlayer = &players[*currentPlayerIndex];
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        currentPlayer->ship.position = GetMousePosition();
        currentPlayer->ship.rotation = (float)GetRandomValue(0, 360);
        wasMouseButtonPressed = true;
    }

    if(wasMouseButtonPressed && CheckCollisionHitboxes(&currentPlayer->ship.hitbox, &currentPlayer->deploymentZone)) {
        currentPlayer->hasDeployed = true;
        *currentPlayerIndex = (*currentPlayerIndex + 1) % 2;
        wasMouseButtonPressed = false;
    }
}

// 
void ShipDeploymentDrawing(Player *players, int* currentPlayerIndex){ // This function goes in UpdateGameDraw() in game.c
    if (*currentPlayerIndex == 0 && !players[0].hasDeployed) {
        DrawText("Player 1", 50, 40, 50, BLUE);
        DrawText(", deploy your ship!", 255, 40, 50, RAYWHITE); // 205 units more that the previous.
    } 
    else if (*currentPlayerIndex == 1 && !players[1].hasDeployed) {
        DrawText("Player 2", 50, 40, 50, RED);
        DrawText(", deploy your ship!", 270, 40, 50, RAYWHITE); // 220 units more that the previous.
    }
}