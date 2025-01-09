#include "game.h"

// Game modules
#include "ship_deployment.h"
#include "movement_commands.h"
#include "movement_half.h"
#include "firing_commands.h"
#include "final_excecution.h"

Player players[2];
Bomb bombs[3];

int currentPlayerIndex = 0; // 0 for player 1 (BLUE), 1 for player 2 (RED)
GamePhase currentPhase = SHIP_DEPLOYMENT;
float originalShipRotation1;
float originalShipRotation2;

void InitPlayers(){
    // Player 1
    players[0].id = 0;
    players[0].numLosses = 0;

    players[0].ship.position = (Vector2){-1, -1};
    players[0].ship.hitbox.rect = (Rectangle){0, 0, 0, 0};
    players[0].ship.rotation = 0.0f;
    players[0].ship.speed = 1.0f;
    players[0].ship.acceleration = 0.0f;
    players[0].ship.color = BLUE;

    players[0].bullet.position = (Vector2){-1, -1};
    players[0].bullet.hitbox.rect = (Rectangle){0, 0, 0, 0};
    players[0].bullet.rotation = 0.0f;
    players[0].bullet.speed = 0.0f;
    players[0].bullet.isActive = false;

    players[0].color = BLUE;
    players[0].hasDeployed = false;
    players[0].hasSelectedDirection = false;
    players[0].hasSelectedSpeed = false;
    players[0].hasFired = false;
    
    // Player 2
    players[1].id = 1;
    players[1].numLosses = 0;

    players[1].ship.position = (Vector2){-1, -1};
    players[1].ship.hitbox.rect = (Rectangle){0, 0, 0, 0};
    players[1].ship.rotation = 0.0f;
    players[1].ship.speed = 1.0f;
    players[1].ship.acceleration = 0.0f;
    players[1].ship.color = RED;

    players[1].bullet.position = (Vector2){-1, -1};
    players[1].bullet.hitbox.rect = (Rectangle){0, 0, 0, 0};
    players[1].bullet.rotation = 0.0f;
    players[1].bullet.speed = 0.0f;
    players[1].bullet.isActive = false;

    players[1].color = RED;
    players[1].hasDeployed = false;
    players[1].hasSelectedDirection = false;
    players[1].hasSelectedSpeed = false;
    players[1].hasFired = false;
}

void InitBombs(){
    bombs[0].position = (Vector2){100, 100};
    bombs[1].position = (Vector2){300, 300};
    bombs[2].position = (Vector2){600, 600};
}

void InitHitboxes() {
    for (int i = 0; i < 2; i++) {
        // Ship Hitboxes
        players[i].ship.hitbox.rect.x = players[i].ship.position.x - players[i].ship.texture.width * SHIP_SCALE / 2.0f;
        players[i].ship.hitbox.rect.y = players[i].ship.position.y - players[i].ship.texture.height * SHIP_SCALE / 2.0f;
        players[i].ship.hitbox.rect.width = players[i].ship.texture.width * SHIP_SCALE;
        players[i].ship.hitbox.rect.height = players[i].ship.texture.height * SHIP_SCALE; // Multiply to scale

        // Bullet Hitboxes
        players[i].bullet.hitbox.rect.x = players[i].bullet.position.x - players[i].bullet.texture.width/ 2.0f;
        players[i].bullet.hitbox.rect.y = players[i].bullet.position.y - players[i].bullet.texture.height / 2.0f;
        players[i].bullet.hitbox.rect.width = players[i].bullet.texture.width+4; 
        players[i].bullet.hitbox.rect.height = players[i].bullet.texture.height+4; 
    }

    for(int i = 0; i < 3; i++){
        // Bomb Hitboxes
        bombs[i].hitbox.rect.x = bombs[i].position.x - bombs[i].texture.width * BOMB_SCALE / 2.0f;
        bombs[i].hitbox.rect.y = bombs[i].position.y - bombs[i].texture.height * BOMB_SCALE / 2.0f;
        bombs[i].hitbox.rect.width = bombs[i].texture.width * BOMB_SCALE;  
        bombs[i].hitbox.rect.height = bombs[i].texture.height * BOMB_SCALE; 
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

void LoadGameTextures(){
    for(int i = 0; i < 2;i++){
        players[i].ship.texture = LoadShipTexture(players[i].color);
        players[i].bullet.texture = LoadTexture("assets/sprites/bullet.png");
    }
    for (int i = 0; i < 3; i++) {
        bombs[i].texture = LoadTexture("assets/sprites/bomb.png");
    }
    LoadSpeedSelectionTextures();
}

void UnloadGameTextures(){
    for(int i = 0; i < 2;i++){
        UnloadTexture(players[i].ship.texture);
        UnloadTexture(players[i].bullet.texture);
    }
    for (int i = 0; i < 3; i++) {
        UnloadTexture(bombs[i].texture);
    }
}

void UpdateHitboxes() {
    for (int i = 0; i < 2; i++) {
        // Ship Hitboxes
        players[i].ship.hitbox.rect.x = players[i].ship.position.x - players[i].ship.texture.width * SHIP_SCALE / 2.0f;
        players[i].ship.hitbox.rect.y = players[i].ship.position.y - players[i].ship.texture.height * SHIP_SCALE / 2.0f;

        // Bullet Hitboxes
        if (players[i].bullet.isActive) {
            players[i].bullet.hitbox.rect.x = players[i].bullet.position.x - 10;
            players[i].bullet.hitbox.rect.y = players[i].bullet.position.y - 10;
        } else {
            players[i].bullet.hitbox.rect.x = -1; // Out of bounds
            players[i].bullet.hitbox.rect.y = -1; // Out of bounds
        }
    }
}

// Debugging function to draw the hitbox
void DrawHitbox(const Hitbox* hitbox) {
    DrawRectangleLinesEx(hitbox->rect, 1.5f, GREEN);
}

// Debugging function to draw the hitboxes
void DrawHitboxes(){
    for(int i = 0; i < 2; i++){
        DrawHitbox(&players[i].ship.hitbox);
        DrawHitbox(&players[i].bullet.hitbox);
    }
    for (int i = 0; i < 3; i++)
    {
        DrawHitbox(&bombs[i].hitbox);
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

void DrawBullets(){
    for (int i = 0; i < 2; i++){
        if (players[i].bullet.isActive){
            DrawTexturePro(
                players[i].bullet.texture,
                (Rectangle){0, 0, players[i].bullet.texture.width, players[i].bullet.texture.height},
                (Rectangle){players[i].bullet.position.x, players[i].bullet.position.y, players[i].bullet.texture.width*BULLET_SCALE, players[i].bullet.texture.height*BULLET_SCALE},
                (Vector2){(players[i].bullet.texture.width*BULLET_SCALE)/2.0f, (players[i].bullet.texture.height*BULLET_SCALE)/2.0f},
                0.0f,
                WHITE
            );
        }
    }
}

void DrawBombs(){
    for (int i = 0; i < 3; i++){
        DrawTexturePro(
            bombs[i].texture,
            (Rectangle){0, 0, bombs[i].texture.width, bombs[i].texture.height},
            (Rectangle){bombs[i].position.x, bombs[i].position.y, bombs[i].texture.width*BOMB_SCALE, bombs[i].texture.height*BOMB_SCALE},
            (Vector2){(bombs[i].texture.width*BOMB_SCALE)/2.0f, (bombs[i].texture.height*BOMB_SCALE)/2.0f},
            0.0f,
            WHITE
        );
    }
}

void HandleGamePhases(){ // TODO: Change bomb / bullet mechanics
    switch (currentPhase) {
        case TITLE_SCREEN:
            //
            break;
        case SHIP_DEPLOYMENT:
            ShipDeploymentInput(players, &currentPlayerIndex);
            ShipDeploymentDrawing(players, &currentPlayerIndex);

            // // Debugging
            // players[0].bullet.isActive = true;
            // players[1].bullet.isActive = true;

            // players[0].bullet.position = (Vector2){500, 500};
            // players[1].bullet.position = (Vector2){500, 700};
            

            if(players[0].hasDeployed && players[1].hasDeployed) currentPhase++;
            break;
        case MOVEMENT_COMMANDS:
            ShipDirectionInput(players, &currentPlayerIndex);
            ShipDirectionDrawing(players, &currentPlayerIndex);

            // Save the original ship rotation
            originalShipRotation1 = players[0].ship.rotation;
            originalShipRotation2 = players[1].ship.rotation;

            if (players[currentPlayerIndex].hasSelectedDirection) ShipSpeedInputDrawing(players, &currentPlayerIndex, 1.0f, 10.0f);
            if (players[0].hasSelectedSpeed && players[1].hasSelectedSpeed) currentPhase++; 
            break;
        case MOVEMENT_HALF:
            if(!players[0].hasFired && !players[1].hasFired){
                UpdateMovementHalfPhase(players, GetFrameTime());

                if (IsMovementPhaseHalfComplete()) {
                    currentPhase = FIRING_COMMANDS;
                }
            }
            break;
        case FIRING_COMMANDS:
            FiringCommandsInput(players, &currentPlayerIndex);
            FiringCommandsDrawing(players, &currentPlayerIndex);
            // Restore the original ship rotation
            if (players[0].hasFired) players[0].ship.rotation = originalShipRotation1;
            if (players[1].hasFired) players[1].ship.rotation = originalShipRotation2;
            if (players[0].hasFired && players[1].hasFired){
                players[0].bullet.isActive = true;
                players[1].bullet.isActive = true;
                currentPhase++;
            } 
            break;
        case FINAL_EXCECUTION:
            FinalExcecution(players);
            UpdateMovementHalfPhase(players, GetFrameTime());
            UpdateBulletMovement(players);
            FiringCommandsDrawing(players, &currentPlayerIndex);
            if (IsMovementPhaseComplete()) {
                // currentPhase = ?;
            }
            break;
        case RESULTS:
            //
            break;
    }
}

// Start is called before the first frame update
void Start(){
    InitPlayers();
    InitBombs();
    LoadGameTextures();
    InitHitboxes(); // Needs to be called after loading textures
}

// Update is called once per frame
void Update(){
    HandleGamePhases();
    UpdateHitboxes();

    DrawBombs();
    DrawShips();
    DrawBullets();
    DrawHitboxes(); // Debugging
}
