#include "game.h"

// Game modules
#include "ship_deployment.h"
#include "movement_commands.h"
#include "movement_half.h"
#include "firing_commands.h"
#include "final_excecution.h"

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
    LoadSpeedSelectionTextures();
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

 // Debugging function to draw the hitboxes
void DrawHitboxes(){
    for(int i = 0; i < 2; i++){
        DrawHitbox(&players[i].ship.hitbox, GREEN);
        DrawHitbox(&players[i].bomb.hitbox, GREEN);
    }
}

void DrawShips(){
    for (int i = 0; i < 2; i++){
        if (players[i].hasDeployed){
            DrawTexturePro(
                players[i].ship.texture,
                (Rectangle){0, 0, players[i].ship.texture.width, players[i].ship.texture.height},
                (Rectangle){players[i].ship.position.x, players[i].ship.position.y, players[i].ship.texture.width*SHIP_SCALE, players[i].ship.texture.height*SHIP_SCALE},
                (Vector2){(players[i].ship.texture.width*SHIP_SCALE)/2.0f, (players[i].ship.texture.height*SHIP_SCALE)/2.0f},
                players[i].ship.rotation,
                WHITE
            );
        }
    }
}

void DrawBombs(){
    for (int i = 0; i < 2; i++){
        if (players[i].hasFired){
            DrawTexturePro(
                players[i].bomb.texture,
                (Rectangle){0, 0, players[i].bomb.texture.width, players[i].bomb.texture.height},
                (Rectangle){players[i].bomb.position.x, players[i].bomb.position.y, players[i].bomb.texture.width*BOMB_SCALE, players[i].bomb.texture.height*BOMB_SCALE},
                (Vector2){(players[i].bomb.texture.width*BOMB_SCALE)/2.0f, (players[i].bomb.texture.height*BOMB_SCALE)/2.0f},
                0.0f,
                WHITE
            );
        }
    }
}

void HandleGamePhases(){
    switch (currentPhase) {
        case TITLE_SCREEN:
            //
            break;
        case SHIP_DEPLOYMENT:
            ShipDeploymentInput(&players, &currentPlayerIndex);
            ShipDeploymentDrawing(&players, &currentPlayerIndex);
            if(players[0].hasDeployed && players[1].hasDeployed) currentPhase++;
            break;
        case MOVEMENT_COMMANDS:
            ShipDirectionInput(&players, &currentPlayerIndex);
            ShipDirectionDrawing(&players, &currentPlayerIndex);
            if (players[currentPlayerIndex].hasSelectedDirection) ShipSpeedInputDrawing(&players, &currentPlayerIndex, 1.0f, 10.0f);
            if (players[0].hasSelectedSpeed && players[1].hasSelectedSpeed) currentPhase++; 
            break;
        case MOVEMENT_HALF:
            if(!players[0].hasFired && !players[1].hasFired){
                UpdateMovementHalfPhase(&players, GetFrameTime());

                if (IsMovementPhaseHalfComplete()) {
                    currentPhase = FIRING_COMMANDS;
                }
                break;
            }
            else{ // After firing commands
                UpdateMovementHalfPhase(&players, GetFrameTime());
                FiringCommandsDrawing(&players, &currentPlayerIndex);
                if (IsMovementPhaseComplete()) {
                    // currentPhase = FINAL_EXCECUTION;
                }
                break;
            }
        case FIRING_COMMANDS:
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
    DrawBombs();
    DrawShips();
    
    HandleGamePhases();
    UpdateHitboxes();
    DrawHitboxes(); // Debugging
}
