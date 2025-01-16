#include "game.h"

// Game modules
#include "ship_deployment.h"
#include "movement_directions.h"
#include "movement.h"
#include "projectile_movement.h"
#include "round_handler.h"
#include "hud.h"

Player players[2];
Bomb bombs[4];

int currentPlayerIndex = 0; // 0 for player 1 (BLUE), 1 for player 2 (RED)
GamePhase currentPhase = TITLE_SCREEN;
bool isRoundOver = false;
float originalShipRotation1;
float originalShipRotation2;

// HUD
Texture backgroundTexture;

// Variables for animation (at victory screen)
Vector2 animatedPosition = { 700, 400 };
Vector2 animatedVelocity = { 200, 150 };
float animatedRotation = 0.0f;

void InitPlayers() {
    // Player 1
    players[0].id = 0;
    players[0].score = 0;

    players[0].ship.position = (Vector2){-1, -1};
    players[0].ship.hitbox.rect = (Rectangle){0, 0, 0, 0};
    players[0].ship.rotation = 0.0f;
    players[0].ship.speed = 1.0f;
    players[0].ship.acceleration = 0.0f;
    players[0].ship.color = BLUE;

    players[0].projectile.position = (Vector2){-1, -1};
    players[0].projectile.hitbox.rect = (Rectangle){0, 0, 0, 0};
    players[0].projectile.rotation = 0.0f;
    players[0].projectile.speed = 0.0f;
    players[0].projectile.isActive = false;
    players[0].projectile.movementAvailableZone.rect = (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    players[0].projectile.isOutOfBounds = false;

    players[0].color = BLUE;
    players[0].hasDeployed = false;
    players[0].hasSelectedDirection = false;
    players[0].hasSelectedSpeed = false;
    players[0].hasFired = false;

    players[0].deploymentZone.rect = (Rectangle){MARGIN_LEFT+60, MARGIN_TOP+70, 300, 670};
    players[0].movementAvailableZone.rect = (Rectangle){MARGIN_LEFT+80, MARGIN_TOP+80, 1550-80*2, 800-80*2};
    
    // Player 2
    players[1].id = 1;
    players[1].score = 0;

    players[1].ship.position = (Vector2){-1, -1};
    players[1].ship.hitbox.rect = (Rectangle){0, 0, 0, 0};
    players[1].ship.rotation = 0.0f;
    players[1].ship.speed = 1.0f;
    players[1].ship.acceleration = 0.0f;
    players[1].ship.color = RED;

    players[1].projectile.position = (Vector2){-1, -1};
    players[1].projectile.hitbox.rect = (Rectangle){0, 0, 0, 0};
    players[1].projectile.rotation = 0.0f;
    players[1].projectile.speed = 0.0f;
    players[1].projectile.isActive = false;
    players[1].projectile.movementAvailableZone.rect = (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    players[1].projectile.isOutOfBounds = false;

    players[1].color = RED;
    players[1].hasDeployed = false;
    players[1].hasSelectedDirection = false;
    players[1].hasSelectedSpeed = false;
    players[1].hasFired = false;

    players[1].deploymentZone.rect = (Rectangle){MARGIN_LEFT+1200, MARGIN_TOP+60, 300, 670};
    players[1].movementAvailableZone.rect = (Rectangle){MARGIN_LEFT+80, MARGIN_TOP+80, 1550-80*2, 800-80*2};
}

void InitBombs() {
    bombs[0].position = (Vector2){100, 100};
    bombs[1].position = (Vector2){300, 300};
    bombs[2].position = (Vector2){600, 600};
    bombs[3].position = (Vector2){900, 900};
}

void InitHitboxes() {
    for (int i = 0; i < 2; i++) {
        // Ship Hitboxes
        players[i].ship.hitbox.rect.x = players[i].ship.position.x - players[i].ship.texture.width * SHIP_SCALE / 2.0f;
        players[i].ship.hitbox.rect.y = players[i].ship.position.y - players[i].ship.texture.height * SHIP_SCALE / 2.0f;
        players[i].ship.hitbox.rect.width = players[i].ship.texture.width * SHIP_SCALE;
        players[i].ship.hitbox.rect.height = players[i].ship.texture.height * SHIP_SCALE;

        // Projectile Hitboxes
        players[i].projectile.hitbox.rect.x = players[i].projectile.position.x - players[i].projectile.texture.width/ 2.0f;
        players[i].projectile.hitbox.rect.y = players[i].projectile.position.y - players[i].projectile.texture.height / 2.0f;
        players[i].projectile.hitbox.rect.width = players[i].projectile.texture.width+4; 
        players[i].projectile.hitbox.rect.height = players[i].projectile.texture.height+4; 
    }

    for(int i = 0; i < 4; i++){
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

void LoadGameTextures() {
    for(int i = 0; i < 2;i++){
        players[i].ship.texture = LoadShipTexture(players[i].color);
        players[i].projectile.texture = LoadTexture("assets/sprites/projectile.png");
    }
    for (int i = 0; i < 4; i++) {
        bombs[i].texture = LoadTexture("assets/sprites/bomb.png");
    }
    LoadSpeedSelectionTextures();
    backgroundTexture = LoadTexture("assets/sprites/background.png");
}

void UnloadGameTextures() {
    for(int i = 0; i < 2;i++){
        UnloadTexture(players[i].ship.texture);
        UnloadTexture(players[i].projectile.texture);
    }
    for (int i = 0; i < 4; i++) {
        UnloadTexture(bombs[i].texture);
    }
    UnloadSpeedSelectionTextures();
    UnloadTexture(backgroundTexture);
}

void UpdateHitboxes() {
    for (int i = 0; i < 2; i++) {
        // Ship Hitboxes
        players[i].ship.hitbox.rect.x = players[i].ship.position.x - players[i].ship.texture.width * SHIP_SCALE / 2.0f;
        players[i].ship.hitbox.rect.y = players[i].ship.position.y - players[i].ship.texture.height * SHIP_SCALE / 2.0f;

        // Projectile Hitboxes
        if (players[i].projectile.isActive) {
            players[i].projectile.hitbox.rect.x = players[i].projectile.position.x - 10;
            players[i].projectile.hitbox.rect.y = players[i].projectile.position.y - 10;
        } else {
            players[i].projectile.hitbox.rect.x = -1; // Out of bounds
            players[i].projectile.hitbox.rect.y = -1; // Out of bounds
        }
    }
}

// Debugging function to draw the hitbox
void DrawHitbox(const Hitbox* hitbox, Color color) {
    DrawRectangleLinesEx(hitbox->rect, 1.5f, color);
}

// Debugging function to draw the hitboxes
void DrawHitboxes() {
    for(int i = 0; i < 2; i++){
        DrawHitbox(&players[i].ship.hitbox, GREEN);
        DrawHitbox(&players[i].projectile.hitbox, GREEN);
    }
    for (int i = 0; i < 4; i++)
    {
        DrawHitbox(&bombs[i].hitbox, GREEN);
    }

    DrawHitbox(&players[0].deploymentZone, BLUE);
    DrawHitbox(&players[1].deploymentZone, RED);
    DrawHitbox(&players[0].movementAvailableZone, ORANGE);
    DrawHitbox(&players[0].projectile.movementAvailableZone, BLACK);

}

void DrawShips() {
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

void DrawProjectiles() {
    for (int i = 0; i < 2; i++){
        if (players[i].projectile.isActive){
            DrawTexturePro(
                players[i].projectile.texture,
                (Rectangle){0, 0, players[i].projectile.texture.width, players[i].projectile.texture.height},
                (Rectangle){players[i].projectile.position.x, players[i].projectile.position.y, players[i].projectile.texture.width*PROJECTILE_SCALE, players[i].projectile.texture.height*PROJECTILE_SCALE},
                (Vector2){(players[i].projectile.texture.width*PROJECTILE_SCALE)/2.0f, (players[i].projectile.texture.height*PROJECTILE_SCALE)/2.0f},
                players[i].projectile.rotation+180,
                WHITE
            );
        }
    }
}

void DrawBombs() {
    for (int i = 0; i < 4; i++){
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

bool CheckCollisionHitboxes(const Hitbox *hitbox1, const Hitbox *hitbox2){
    return CheckCollisionRecs(hitbox1->rect, hitbox2->rect);
}

void ResetRound(Player *players) {
    // Reset Players
    for (int i = 0; i < 2; i++)
    {
        // Reset Players
        players[i].hasDeployed = false;
        players[i].hasSelectedDirection = false;
        players[i].hasSelectedSpeed = false;
        players[i].hasFired = false;
        players[i].isHit = false;

        // Reset Projectiles
        players[i].projectile.isActive = false;
        players[i].projectile.isOutOfBounds = false;
    }

    // Reset Timer
    timer = 10.0f;
}

bool CheckGameIsOver(Player *players) {
    if (players[0].score == 3) {
        printf("Player 1 won the game!\n");
        return true;
    } else if (players[1].score == 3) {
        printf("Player 2 won the game!\n");
        return true;
    } else return false;
}

void HandleGamePhases() {
    switch (currentPhase) {
        case TITLE_SCREEN:
            //
            break;
        case SHIP_DEPLOYMENT:
            ShipDeploymentInput(players, &currentPlayerIndex);
            ShipDeploymentDrawing(players, &currentPlayerIndex);

            if(players[0].hasDeployed && players[1].hasDeployed) currentPhase++;
            break;
        case MOVEMENT_DIRECTIONS:
            ShipDirectionInput(players, &currentPlayerIndex);
            ShipDirectionDrawing(players, &currentPlayerIndex);

            // Save the original ship rotation
            originalShipRotation1 = players[0].ship.rotation;
            originalShipRotation2 = players[1].ship.rotation;

            if (players[currentPlayerIndex].hasSelectedDirection) ShipSpeedInputDrawing(players, &currentPlayerIndex, 1.0f, 10.0f);
            if (players[0].hasSelectedSpeed && players[1].hasSelectedSpeed) currentPhase++; 
            break;
        case MOVEMENT:
            if(!players[0].hasFired && !players[1].hasFired){
                UpdateMovement(players, GetFrameTime());

                if (IsMovementPhaseHalfComplete()) currentPhase++;
            }
            break;
        case PROJECTILE_MOVEMENT:
            FiringCommandsInput(players, &currentPlayerIndex);
            FiringCommandsDrawing(players, &currentPlayerIndex);
            // Restore the original ship rotation
            if (players[0].hasFired) players[0].ship.rotation = originalShipRotation1;
            if (players[1].hasFired) players[1].ship.rotation = originalShipRotation2;
            if (players[0].hasFired && players[1].hasFired){
                players[0].projectile.isActive = true;
                players[1].projectile.isActive = true;
                currentPhase++;
            } 
            break;
        case ROUND_HANDLING:
            if (players[0].projectile.isOutOfBounds && players[1].projectile.isOutOfBounds) isRoundOver = true;
            CheckRoundWinCondition(players);
            UpdateMovement(players, GetFrameTime());
            UpdateProjectileMovement(players);
            FiringCommandsDrawing(players, &currentPlayerIndex);
            if (IsMovementPhaseComplete()) {
                // currentPhase = ?;
            }
            break;
    }
}

void UpdateAnimatedPlayer(float deltaTime) {
    // Update position
    animatedPosition.x += animatedVelocity.x * deltaTime;
    animatedPosition.y += animatedVelocity.y * deltaTime;

    // Check for collision with screen edges and bounce
    if (animatedPosition.x <= 0 || animatedPosition.x + players[0].ship.texture.width*0.5f >= GetScreenWidth()) {
        animatedVelocity.x *= -1;
    }
    if (animatedPosition.y<= 0 || animatedPosition.y + players[0].ship.texture.height*0.5f >= GetScreenHeight()) {
        animatedVelocity.y *= -1;
    }

    // Update rotation based on velocity direction
    animatedRotation = atan2(animatedVelocity.y, animatedVelocity.x) * RAD2DEG - 90;
}

// Start is called before the first frame update
void Start() {
    InitPlayers();
    InitBombs();
    LoadGameTextures();
    InitHitboxes(); // Needs to be called after loading textures
}

// Update is called once per frame
void Update() {
    if (!CheckGameIsOver(players) && currentPhase != TITLE_SCREEN) {
        ClearBackground(RAYWHITE);
        DrawBackground(&backgroundTexture);
        DrawScore(players);

        if (isRoundOver) { // Restart the game to the next round
            ResetRound(players);
            currentPhase = SHIP_DEPLOYMENT;
            isRoundOver = false;
        }

        HandleGamePhases();
        UpdateHitboxes();

        // DrawBombs();
        DrawShips();
        DrawProjectiles();

        DrawHitboxes(); // Debugging
    } else if(CheckGameIsOver(players) && currentPhase != TITLE_SCREEN) {
        ClearBackground(LIGHTGRAY);
        if(players[0].score == 3){
            UpdateAnimatedPlayer(GetFrameTime());
            DrawTextureEx(players[0].ship.texture, animatedPosition, animatedRotation, 10, WHITE);
            DrawText("Player 1", 700, 250, 70, BLUE);
            DrawText(" has won the game! Congratulations!", 220, 350, 70, YELLOW);
        } else if (players[1].score == 3) {
            UpdateAnimatedPlayer(GetFrameTime());
            DrawTextureEx(players[1].ship.texture, animatedPosition, animatedRotation, 10, WHITE);
            DrawText("Player 2", 700, 250, 70, RED);
            DrawText(" has won the game! Congratulations!", 220, 350, 70, YELLOW);
        }
    } else if(currentPhase == TITLE_SCREEN) {
        ClearBackground(LIGHTGRAY);
        
    }
}