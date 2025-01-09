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
#define SHIP_SCALE 5.0f
#define BULLET_SCALE 2.5f
#define BOMB_SCALE 2.0f

#define BULLET_SPEED 1.5f

#define SHORE_MARGIN 100 // Margin in pixels from the edge

// Enums
typedef enum GamePhase {TITLE_SCREEN, SHIP_DEPLOYMENT, MOVEMENT_COMMANDS, MOVEMENT_HALF, FIRING_COMMANDS, FINAL_EXCECUTION, RESULTS} GamePhase;

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

typedef struct Bullet {
    Vector2 position;
    Hitbox hitbox;
    float rotation;
    float speed;
    bool isActive;
    Texture2D texture;
} Bullet;
    

typedef struct Bomb {
    Vector2 position;
    Hitbox hitbox;
    Texture2D texture;
} Bomb;

typedef struct Player{
    int id;
    Ship ship;
    Bullet bullet;
    Color color;
    bool hasDeployed;
    bool hasSelectedDirection;
    bool hasSelectedSpeed;
    bool hasFired;
} Player;


// Functions
void Start();
void Update();

void UnloadGameTextures();

#endif // GAME_H