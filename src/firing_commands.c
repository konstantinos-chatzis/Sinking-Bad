#include "firing_commands.h"

void FiringCommandsInput(Player *players, int* currentPlayerIndex){
    Player* currentPlayer = &players[*currentPlayerIndex];
    if(!currentPlayer->hasFired){
        Vector2 mousePosition = GetMousePosition();
        currentPlayer->ship.rotation = atan2(currentPlayer->ship.position.y - mousePosition.y, currentPlayer->ship.position.x - mousePosition.x)*RAD2DEG+90;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            // Set bullet position
            currentPlayer->bullet.position = currentPlayer->ship.position;

            // Set bullet rotation (direction)
            currentPlayer->bullet.rotation = currentPlayer->ship.rotation;

            currentPlayer->hasFired = true;
            *currentPlayerIndex = (*currentPlayerIndex + 1) % 2;
        }
    }
}

void UpdateBulletMovement(Player *players){
    for (int i = 0; i < 2; i++){
        if (players[i].hasFired){
            // Calculate bullet movement based on rotation and speed
            float dx = cos(players[i].bullet.rotation * DEG2RAD + 90*DEG2RAD) * BULLET_SPEED;
            float dy = sin(players[i].bullet.rotation * DEG2RAD + 90*DEG2RAD) * BULLET_SPEED;

            // Update bullet position
            players[i].bullet.position.x += dx;
            players[i].bullet.position.y += dy;
        }
    }
}

void FiringCommandsDrawing(Player *players, int* currentPlayerIndex){
    if (*currentPlayerIndex == 0 && !players[0].hasFired) {
        DrawText("Player 1", 50, 50, 50, BLUE);
        DrawText(", fire your bullet!", 255, 50, 50, GRAY); // 205 units more that the previous.
    } 
    else if (*currentPlayerIndex == 1 && !players[1].hasFired) {
        DrawText("Player 2", 50, 50, 50, RED);
        DrawText(", fire your bullet!", 270, 50, 50, GRAY); // 220 units more that the previous.
    }
}
