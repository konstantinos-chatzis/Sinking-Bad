#include <game.h>
#include <movement_half.h>

float movementTime = 10.0f; // Time in seconds for the phase
float elapsedTime = 0.0f; // Time elapsed since the start of the phase

float ComputeAcceleration(float initialSpeed, float D_min, float D_max, float k, float p) {
    float normalizedDistance = D_min + (D_max - D_min) * powf(initialSpeed, p) / (powf(initialSpeed, p) + k);

    // Calculate required average deceleration to cover normalizedDistance
    float requiredDeceleration = (initialSpeed * initialSpeed) / (2.0f * normalizedDistance);
    return -requiredDeceleration; // Always negative
}


void UpdateMovementHalfPhase(Player *players, float deltaTime) {
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
        if (players[i].hasDeployed && players[i].hasSelectedDirection && players[i].hasSelectedSpeed) {
            
            // Parameters for balancing
            float D_min = 7.5f;    // Minimum travel distance for the slowest ship
            float D_max = 15.0f;   // Maximum travel distance for the fastest ship
            float k = 8.0f;         // Scaling factor for the turning point
            float p = 5.0f;         // Steepness of the curve

            players[i].ship.acceleration = ComputeAcceleration(players[i].ship.speed, D_min, D_max, k, p);
            players[i].ship.speed += players[i].ship.acceleration*deltaTime;

            // Ensure the speed doesn't go below zero
            if (players[i].ship.speed < 0.0f) {
                players[i].ship.speed = 0.0f;
            }

            // Calculate movement based on rotation and speed
            float dx = cos(players[i].ship.rotation * DEG2RAD + 90*DEG2RAD) * players[i].ship.speed/2.5;
            float dy = sin(players[i].ship.rotation * DEG2RAD + 90*DEG2RAD) * players[i].ship.speed/2.5;
            
            // Update ship position
            players[i].ship.position.x += dx;
            players[i].ship.position.y += dy;

            // Check boundaries
            if (players[i].ship.position.x < SHORE_MARGIN) {
                players[i].ship.position.x = SHORE_MARGIN;
                players[i].ship.speed = 0; // Stop the ship (or reverse direction if desired)
            }
            if (players[i].ship.position.x > (SCREEN_WIDTH - SHORE_MARGIN)) {
                players[i].ship.position.x = SCREEN_WIDTH - SHORE_MARGIN;
                players[i].ship.speed = 0;
            }
            if (players[i].ship.position.y < SHORE_MARGIN) {
                players[i].ship.position.y = SHORE_MARGIN;
                players[i].ship.speed = 0;
            }
            if (players[i].ship.position.y > (SCREEN_HEIGHT - SHORE_MARGIN)) {
                players[i].ship.position.y = SCREEN_HEIGHT - SHORE_MARGIN;
                players[i].ship.speed = 0;
            }
        }
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

