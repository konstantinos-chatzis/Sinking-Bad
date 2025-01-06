#include <game.h>
#include <phase_movement_half.h>

float movementTime = 10.0f; // Time in seconds for the phase
float elapsedTime = 0.0f; // Time elapsed since the start of the phase

float ComputeAcceleration(float initialSpeed, float D_min, float D_max, float k, float p) {
    float normalizedDistance = D_min + (D_max - D_min) * powf(initialSpeed, p) / (powf(initialSpeed, p) + k);

    // Calculate required average deceleration to cover normalizedDistance
    float requiredDeceleration = (initialSpeed * initialSpeed) / (2.0f * normalizedDistance);
    return -requiredDeceleration; // Always negative
}


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
            
            // Parameters for balancing
            float D_min = 5.0f;    // Minimum travel distance for the slowest ship
            float D_max = 10.0f;   // Maximum travel distance for the fastest ship
            float k = 6.0f;         // Scaling factor for the turning point
            float p = 15.0f;         // Steepness of the curve

            currentPlayer->ship.acceleration = ComputeAcceleration(currentPlayer->ship.speed, D_min, D_max, k, p);
            currentPlayer->ship.speed += currentPlayer->ship.acceleration*deltaTime;

            // Ensure the speed doesn't go below zero
            if (currentPlayer->ship.speed < 0.0f) {
                currentPlayer->ship.speed = 0.0f;
            }

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
            (Rectangle){(*players)[i].ship.position.x, (*players)[i].ship.position.y, (*players)[i].ship.texture.width * 5, (*players)[i].ship.texture.height * 5},
            (Vector2){((*players)[i].ship.texture.width * 5) / 2.0f, ((*players)[i].ship.texture.height * 5) / 2.0f},
            (*players)[i].ship.rotation,
            WHITE
        );
    }
}

bool IsMovementPhaseHalfComplete() {
    if (movementTime <= 5) return true;
    else return false;
}

bool IsMovementPhaseComplete() {
    if (movementTime <= 0) return true;
    else return false;
}

