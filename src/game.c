#include "game.h"

// Game modules
#include "ship_deployment.h"
#include "movement_commands.h"
#include "movement_half.h"
#include "firing_commands.h"


Player players[2] = {
    {
        .id=0,
        .ship={
            {0, 0}, {0, 0, 0, 0}, 0.0f, 1.0f, 0.0f, BLUE
        },
        .bomb={{0, 0}, {0, 0, 0, 0}, false, false},
        .color=BLUE,
        .hasDeployed=false,
        .hasSelectedDirection=false,
        .hasSelectedSpeed=false
    },
    {
        .id=1,
        .ship={{0, 0}, {0, 0, 0, 0}, 0.0f, 1.0f, 0.0f, RED},
        .bomb={{0, 0}, {0, 0, 0, 0}, false, false},
        .color=RED,
        .hasDeployed=false,
        .hasSelectedDirection=false,
        .hasSelectedSpeed=false
    },
};

int currentPlayerIndex = 0; // 0 for player 1 (BLUE), 1 for player 2 (RED)
GamePhase currentPhase = SHIP_DEPLOYMENT;

void InitHitboxes() {
    for (int i = 0; i < 2; i++) {
        // Ship Hitboxes
        players[i].ship.hitbox.x = players[i].ship.position.x;
        players[i].ship.hitbox.y = players[i].ship.position.y;
        players[i].ship.hitbox.width = players[i].ship.texture.width * 2.6;  // Multiply by 5 to scale
        players[i].ship.hitbox.height = players[i].ship.texture.height * 5; // Multiply by 5 to scale
        players[i].ship.hitbox.rotation = players[i].ship.rotation; // Set the rotation

        // Bomb Hitboxes
        players[i].bomb.hitbox.x = players[i].bomb.position.x;
        players[i].bomb.hitbox.y = players[i].bomb.position.y;
        players[i].bomb.hitbox.width = players[i].bomb.texture.width * 1.5;  // Multiply by 1.5 to scale
        players[i].bomb.hitbox.height = players[i].bomb.texture.height * 1.5; // Multiply by 1.5 to scale
        players[i].bomb.hitbox.rotation = 0.0f; // Set the rotation
    }
}

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

Texture2D LoadBombTexture(Color color) {
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
    snprintf(filePath, sizeof(filePath), "assets/sprites/bomb_%s.png", colorName);

    // Load and return the texture
    return LoadTexture(filePath);
}

void LoadGameTextures(){
    for(int i = 0; i < 2;i++){
        players[i].ship.texture = LoadShipTexture(players[i].ship.color);
        players[i].bomb.texture = LoadBombTexture(players[i].color);
    }
}

void UnloadGameTextures(){
    for(int i = 0; i < 2;i++){
        UnloadTexture(players[i].ship.texture);
        UnloadTexture(players[i].bomb.texture);
    }
}

void UpdateHitboxes() {
    for (int i = 0; i < 2; i++) {
        // Ship Hitboxes
        players[i].ship.hitbox.x = players[i].ship.position.x;
        players[i].ship.hitbox.y = players[i].ship.position.y;
        players[i].ship.hitbox.rotation = players[i].ship.rotation;

        // Bomb Hitboxes
        players[i].bomb.hitbox.x = players[i].bomb.position.x;
        players[i].bomb.hitbox.y = players[i].bomb.position.y;
    }
}

// Helper function to get the corners of a rotated hitbox
void GetHitboxCorners(const Hitbox* hitbox, Vector2 corners[4]) {
    float hw = hitbox->width / 2.0f;  // Half width
    float hh = hitbox->height / 2.0f; // Half height
    float angleRad = hitbox->rotation * DEG2RAD;  // Convert degrees to radians

    // Corner offsets before rotation (from the center of the hitbox)
    Vector2 offsets[4] = {
        {-hw, -hh}, // Top-left
        { hw, -hh}, // Top-right
        { hw,  hh}, // Bottom-right
        {-hw,  hh}  // Bottom-left
    };

    // Rotate each corner and apply the position of the hitbox
    for (int i = 0; i < 4; i++) {
        // Rotate the corner
        float rotatedX = offsets[i].x * cos(angleRad) - offsets[i].y * sin(angleRad);
        float rotatedY = offsets[i].x * sin(angleRad) + offsets[i].y * cos(angleRad);
    
        // Add the hitbox position to the rotated corners
        corners[i].x = hitbox->x + rotatedX;
        corners[i].y = hitbox->y + rotatedY;
    }
}

// Project polygon corners onto an axis and find the minimum and maximum projections
void ProjectPolygon(const Vector2* corners, int numCorners, Vector2 axis, float* min, float* max) {
    *min = FLT_MAX;
    *max = -FLT_MAX;

    for (int i = 0; i < numCorners; i++) {
        // Projection of the corner onto the axis
        float projection = corners[i].x * axis.x + corners[i].y * axis.y; // Dot product
        if (projection < *min) *min = projection;
        if (projection > *max) *max = projection;
    }
}

// Check for overlap between two projected ranges
bool CheckOverlap(float minA, float maxA, float minB, float maxB) {
    return !(maxA < minB || maxB < minA); // No gap means overlap
}

// Main SAT collision detection function
bool CheckSATCollision(const Hitbox* hitboxA, const Hitbox* hitboxB) {
    // Get the corners of both hitboxes
    Vector2 cornersA[4], cornersB[4];
    GetHitboxCorners(hitboxA, cornersA);
    GetHitboxCorners(hitboxB, cornersB);

    // Check for overlap along each edge of both polygons
    Vector2 axes[4];
    
    // Get the edges and the axes perpendicular to them
    for (int i = 0; i < 4; i++) {
        // Edge for hitbox A
        Vector2 edgeA = (i == 3) ? Vector2Subtract(cornersA[0], cornersA[3]) : Vector2Subtract(cornersA[i + 1], cornersA[i]);
        axes[i] = (Vector2){-edgeA.y, edgeA.x};  // Perpendicular to the edge
    }

    // Check for overlap along each axis
    for (int i = 0; i < 4; i++) {
        // Project hitbox A and B onto the axis
        float minA, maxA, minB, maxB;
        ProjectPolygon(cornersA, 4, axes[i], &minA, &maxA);
        ProjectPolygon(cornersB, 4, axes[i], &minB, &maxB);

        // Check if there's no overlap
        if (!CheckOverlap(minA, maxA, minB, maxB)) {
            return false;  // No collision if there's no overlap on this axis
        }
    }

    // If no separating axis is found, there is a collision
    return true;
}

// Debugging function to draw the hitbox
void DrawHitbox(const Hitbox* hitbox, Color color) {
    // Get the corners of the hitbox
    Vector2 corners[4];
    GetHitboxCorners(hitbox, corners);

    // Draw the edges of the rectangle
    for (int i = 0; i < 4; i++) {
        Vector2 start = corners[i];
        Vector2 end = corners[(i + 1) % 4];  // Wrap around to connect the last point to the first
        
        // Draw line between the two corners
        DrawLineV(start, end, color);
    }
}


void HandleGamePhases(){
    switch (currentPhase) {
        case TITLE_SCREEN:
            //
            break;
        case SHIP_DEPLOYMENT:
            UpdateHitboxes();
            DrawHitbox(&players[0].ship.hitbox, GREEN); // Debugging
            DrawHitbox(&players[1].ship.hitbox, GREEN); // Debugging
            DrawHitbox(&players[0].bomb.hitbox, GREEN); // Debugging
            DrawHitbox(&players[1].bomb.hitbox, GREEN); // Debugging
            ShipDeploymentInput(&players, &currentPlayerIndex);
            ShipDeploymentDrawing(&players, &currentPlayerIndex);
            if(players[0].hasDeployed && players[1].hasDeployed) currentPhase++;
            break;
        case MOVEMENT_COMMANDS:
            UpdateHitboxes();
            DrawHitbox(&players[0].ship.hitbox, GREEN); // Debugging
            DrawHitbox(&players[1].ship.hitbox, GREEN); // Debugging
            DrawHitbox(&players[0].bomb.hitbox, GREEN); // Debugging
            DrawHitbox(&players[1].bomb.hitbox, GREEN); // Debugging
            ShipDirectionInput(&players, &currentPlayerIndex);
            ShipDirectionDrawing(&players, &currentPlayerIndex);
            if (players[currentPlayerIndex].hasSelectedDirection) ShipSpeedInputDrawing(&players, &currentPlayerIndex, 1.0f, 10.0f);
            if (players[0].hasSelectedSpeed && players[1].hasSelectedSpeed) currentPhase++; 
            break;
        case MOVEMENT_HALF:
            if(!players[0].hasFired && !players[1].hasFired){
                UpdateHitboxes();
                DrawHitbox(&players[0].ship.hitbox, GREEN); // Debugging
                DrawHitbox(&players[1].ship.hitbox, GREEN); // Debugging
                DrawHitbox(&players[0].bomb.hitbox, GREEN); // Debugging
                DrawHitbox(&players[1].bomb.hitbox, GREEN); // Debugging
                UpdateMovementHalfPhase(&players, GetFrameTime());
                DrawMovementHalfPhase(&players);

                if (IsMovementPhaseHalfComplete()) {
                    currentPhase = FIRING_COMMANDS;
                }
                break;
            }
            else{ // After firing commands
                UpdateHitboxes();
                DrawHitbox(&players[0].ship.hitbox, GREEN); // Debugging
                DrawHitbox(&players[1].ship.hitbox, GREEN); // Debugging
                DrawHitbox(&players[0].bomb.hitbox, GREEN); // Debugging
                DrawHitbox(&players[1].bomb.hitbox, GREEN); // Debugging
                UpdateMovementHalfPhase(&players, GetFrameTime());
                DrawMovementHalfPhase(&players);
                FiringCommandsDrawing(&players, &currentPlayerIndex);
                if (IsMovementPhaseComplete()) {
                    // currentPhase = FINAL_EXCECUTION;
                }
                break;
            }
        case FIRING_COMMANDS:
            UpdateHitboxes();
            DrawHitbox(&players[0].ship.hitbox, GREEN); // Debugging
            DrawHitbox(&players[1].ship.hitbox, GREEN); // Debugging
            DrawHitbox(&players[0].bomb.hitbox, GREEN); // Debugging
            DrawHitbox(&players[1].bomb.hitbox, GREEN); // Debugging
            FiringCommandsInput(&players, &currentPlayerIndex);
            FiringCommandsDrawing(&players, &currentPlayerIndex);
            if (players[0].hasFired && players[1].hasFired) currentPhase = MOVEMENT_HALF;
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
