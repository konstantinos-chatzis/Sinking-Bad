#include <game.h>
#include <phase_movement_half.h>

float movementTime = 6.0f; // Time in seconds for the movement half phase
float elapsedTime = 0.0f;

void UpdateMovementHalfPhase(Player (*players)[2], float deltaTime) {
    // Currently, this is where you can add logic for updating movement or other gameplay mechanics.
    // Ships are already moving in the main phase handling.
    elapsedTime += deltaTime;
    if (elapsedTime >= 1.0f) {
        if (movementTime > 0.0f) { // Decrease time until it reaches 0
            printf("Movement Half Phase Time: %.1f\n", movementTime);
            movementTime -= 1.0f;
            elapsedTime = 0.0f;
        }
    }

    // Update ship movement based on their rotation and speed
    for (int i = 0; i < 2; i++) {
        Player* currentPlayer = &(*players)[i];
        if (currentPlayer->hasDeployed && currentPlayer->hasSelectedDirection && currentPlayer->hasSelectedSpeed) {
            // Calculate movement based on rotation and speed
            float dx = cos(currentPlayer->ship.rotation * DEG2RAD + 90*DEG2RAD) * currentPlayer->ship.speed/2.5;
            float dy = sin(currentPlayer->ship.rotation * DEG2RAD + 90*DEG2RAD) * currentPlayer->ship.speed/2.5;
            
            // Update the ship's position
            currentPlayer->ship.position.x += dx;
            currentPlayer->ship.position.y += dy;
        }
    }
}

void DrawMovementHalfPhase(Player (*players)[2]) {
    // Draw ships as they move
    for (int i = 0; i < 2; i++) {
        DrawTexturePro(
            (*players)[i].ship.texture,
            (Rectangle){0, 0, (*players)[i].ship.texture.width, (*players)[i].ship.texture.height},
            (Rectangle){(*players)[i].ship.position.x, (*players)[i].ship.position.y, (*players)[i].ship.texture.width*5, (*players)[i].ship.texture.height*5},
            (Vector2){((*players)[i].ship.texture.width*5)/2.0f, ((*players)[i].ship.texture.height*5)/2.0f},
            (*players)[i].ship.rotation,
            WHITE
        );
    }
}

bool IsMovementHalfPhaseComplete() {
    if (movementTime <= 2.5) return true;
    else return false;
}
