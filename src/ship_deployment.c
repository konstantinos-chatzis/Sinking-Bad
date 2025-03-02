#include "ship_deployment.h"

bool wasMouseButtonPressed = false;
// Get mouse input for ship deployment
void ShipDeploymentInput(Player *players, int* currentPlayerIndex) { // This function goes in UpdateGameInput() in game.c
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

//  Draw suggestive text for ship deployment
void ShipDeploymentDrawing(Player *players, int* currentPlayerIndex) { // This function goes in UpdateGameDraw() in game.c
    if (*currentPlayerIndex == 0 && !players[0].hasDeployed) {
        DrawText("Player 1", 50, 40, 50, BLUE);
        DrawText(", deploy your ship!", 255, 40, 50, RAYWHITE); // 205 units more that the previous.

        DrawRectangleRec(players[0].deploymentZone.rect, (Color){0, 121, 241, 128});
    } 
    else if (*currentPlayerIndex == 1 && !players[1].hasDeployed) {
        DrawText("Player 2", 50, 40, 50, RED);
        DrawText(", deploy your ship!", 270, 40, 50, RAYWHITE); // 220 units more that the previous.

        DrawRectangleRec(players[1].deploymentZone.rect, (Color){130, 41, 55, 128});
    }
}