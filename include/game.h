#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <raymath.h>

// Constants
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900



// Enums
typedef enum GamePhase {SHIP_DEPLOYMENT, MOVEMENT_COMMANDS, FIRING_COMMANDS, FINAL_EXCECUTION, RESULTS} GamePhase;

//Structs
typedef struct Ship {
    Vector2 position;
    float rotation;
    bool deployed;
    Color color;
    Texture2D texture;
} Ship;

typedef struct Player{
    int id;
    Ship ship;
    Color color;
} Player;

// Functions

void UpdateGameInput();
void UpdateGameDraw();
void LoadGameTextures();
void UnloadGameTextures();

// For GameState SHIP_DEPLOYMENT
void ShipDeploymentInput();
void ShipDeploymentDrawing();


#endif