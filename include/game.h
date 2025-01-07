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
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

// Enums
typedef enum GamePhase {TITLE_SCREEN, SHIP_DEPLOYMENT, MOVEMENT_COMMANDS, MOVEMENT_HALF, FIRING_COMMANDS, MOVEMENT_REST, FINAL_EXCECUTION, RESULTS} GamePhase;

//Structs
typedef struct Hitbox {
    float x;
    float y;
    float width;
    float height;
    float rotation;
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

typedef struct Bomb {
    Vector2 position;
    Hitbox hitbox;
    bool isVisible;
    bool isActive;
    Texture2D texture;
} Bomb;

typedef struct Player{
    int id;
    Ship ship;
    Bomb bomb;
    Color color;
    bool hasDeployed;
    bool hasSelectedDirection;
    bool hasSelectedSpeed;
    bool hasFired;
} Player;


// Functions
void InitHitboxes();
void Update();
void LoadGameTextures();
void UnloadGameTextures();

#endif // GAME_H