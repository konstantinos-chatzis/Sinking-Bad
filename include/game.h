#ifndef GAME_H
#define GAME_H

// External Libraries
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

// Constants
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

// Enums
typedef enum GamePhase {TITLE_SCREEN, SHIP_DEPLOYMENT, MOVEMENT_COMMANDS, MOVEMENT_HALF, FIRING_COMMANDS, MOVEMENT_REST, FINAL_EXCECUTION, RESULTS} GamePhase;

//Structs
typedef struct Ship {
    Vector2 position;
    float rotation;
    float speed;
    float acceleration;
    Color color;
    Texture2D texture;
} Ship;

typedef struct Player{
    int id;
    Ship ship;
    Color color;
    bool hasDeployed;
    bool hasSelectedDirection;
    bool hasSelectedSpeed;
} Player;

// Functions
void Update();
void LoadGameTextures();
void UnloadGameTextures();


#endif