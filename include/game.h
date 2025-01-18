#ifndef GAME_H
#define GAME_H

// External Libraries
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdbool.h> 

// Constants
#define SCREEN_WIDTH 1760
#define SCREEN_HEIGHT 990
#define SHIP_SCALE 5.0f
#define PROJECTILE_SCALE 2.0f
#define BOMB_SCALE 2.0f

#define PROJECTILE_SPEED 1.8f

#define MARGIN_LEFT 100 // Margin in pixels from the left
#define MARGIN_TOP 100 // Margin in pixels from the top

// Global Variables
extern bool isRoundOver;
extern float timer;
extern bool isGamePaused;

// Enums
typedef enum GamePhase {TITLE_SCREEN, SHIP_DEPLOYMENT, MOVEMENT_DIRECTIONS, MOVEMENT, PROJECTILE_MOVEMENT, ROUND_HANDLING} GamePhase;

//Structs
typedef struct Hitbox {
    Rectangle rect;
} Hitbox;

typedef struct Ship {
    Vector2 position;
    Hitbox hitbox;
    float rotation;
    float speed;
    float acceleration;
    Color color;
    Texture2D texture;
} Ship;

typedef struct Projectile {
    Vector2 position;
    Hitbox hitbox;
    float rotation;
    float speed;
    bool isActive;
    Texture2D texture;
    Hitbox movementAvailableZone;
    bool isOutOfBounds;
} Projectile;
    
typedef struct Bomb {
    Vector2 position;
    Hitbox hitbox;
    Texture2D texture;
} Bomb;

typedef struct Player{
    int id;
    Ship ship;
    Projectile projectile;
    Color color;
    bool hasDeployed;
    bool hasSelectedDirection;
    bool hasSelectedSpeed;
    bool hasFired;
    bool isHit;
    int score;
    Hitbox deploymentZone;
    Hitbox movementAvailableZone;
} Player;

// Functions
void Start();
void Update();

bool CheckCollisionHitboxes(const Hitbox *hitbox1, const Hitbox *hitbox2);

void UnloadGameTextures();

#endif // GAME_H