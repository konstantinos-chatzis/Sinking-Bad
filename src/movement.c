#include <movement.h>

float timer = 10.0f; // Time in seconds for the phase
float elapsedTime = 0.0f; // Time elapsed since the start of the phase

// Computes the ships required negative accelleration based on its initial speed. Returns that acceleration
float ComputeAcceleration(float initialSpeed, float D_min, float D_max, float k, float p) {
    float normalizedDistance = D_min + (D_max - D_min) * powf(initialSpeed, p) / (powf(initialSpeed, p) + k);

    // Calculate required average deceleration to cover normalizedDistance
    float requiredDeceleration = (initialSpeed * initialSpeed) / (2.0f * normalizedDistance);
    return -requiredDeceleration; // Always negative
}

// Update the timer, and the ship's positions based on their speed and acceleration
void UpdateMovement(Player *players, float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= 1.0f) {
        if (timer > 0.0f) { // Decrease time until it reaches 0
            printf("Movement Half Phase Time: %.1f\n", timer);
            timer -= 1.0f;
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

            // Check if ship is within the movement zone
            if (!CheckCollisionHitboxes(&players[i].ship.hitbox, &players[i].movementAvailableZone)) {
                players[i].ship.position.x -= dx;
                players[i].ship.position.y -= dy;
                players[i].ship.speed = 0;
            }
        }
    }
}

// Check if the timer is half done. If so, returns true
bool IsMovementPhaseHalfComplete() {
    if (timer <= 5) return true;
    else return false;
}