#include "projectile_movement.h"

void FiringCommandsInput(Player *players, int* currentPlayerIndex){
    Player* currentPlayer = &players[*currentPlayerIndex];
    if(!currentPlayer->hasFired){
        Vector2 mousePosition = GetMousePosition();
        currentPlayer->ship.rotation = atan2(currentPlayer->ship.position.y - mousePosition.y, currentPlayer->ship.position.x - mousePosition.x)*RAD2DEG+90;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            // Set projectile position
            currentPlayer->projectile.position = currentPlayer->ship.position;

            // Set projectile rotation (direction)
            currentPlayer->projectile.rotation = currentPlayer->ship.rotation;

            currentPlayer->hasFired = true;
            *currentPlayerIndex = (*currentPlayerIndex + 1) % 2;
        }
    }
}

void UpdateProjectileMovement(Player *players){
    for (int i = 0; i < 2; i++){
        if (players[i].hasFired){
            // Calculate projectile movement based on rotation and speed
            float dx = cos(players[i].projectile.rotation * DEG2RAD + 90*DEG2RAD) * PROJECTILE_SPEED;
            float dy = sin(players[i].projectile.rotation * DEG2RAD + 90*DEG2RAD) * PROJECTILE_SPEED;

            // Update projectile position
            players[i].projectile.position.x += dx;
            players[i].projectile.position.y += dy;

            if(!CheckCollisionHitboxes(&players[i].projectile.hitbox, &players[i].projectile.movementAvailableZone)){
                players[i].projectile.isOutOfBounds = true;
            } 
        }
    }
}

void FiringCommandsDrawing(Player *players, int* currentPlayerIndex){
    Player *currentPlayer = &players[*currentPlayerIndex];

    if (!currentPlayer->hasFired){
        if (*currentPlayerIndex == 0) {
            DrawText("Player 1", 50, 50, 50, BLUE);
            DrawText(", fire your projectile!", 255, 50, 50, GRAY); // 205 units more that the previous.
        } 
        else if (*currentPlayerIndex == 1) {
            DrawText("Player 2", 50, 50, 50, RED);
            DrawText(", fire your projectile!", 270, 50, 50, GRAY); // 220 units more that the previous.
        }

        // Draw aiming line
        Vector2 mousePosition = GetMousePosition();
        DrawLineEx(currentPlayer->ship.position, mousePosition, 2.0f, GRAY);
        DrawCircleV(mousePosition, 5, GRAY);
    }
}
