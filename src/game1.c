#include <raylib.h>
#include <game1.h>

Obstacle obstacles[MAX_OBSTACLES];

// Initializes a ship at a specific point.
Ship InitShip(int startX, int startY){
    return (Ship){
        .x = startX,
        .y = startY,
        .velocity = 0,
        .direction = 0,
        .isMoving = false
    };
}

// Initializes a bullet that is not yet on the grid.
Bullet InitBullet(){
    return (Bullet){
        .x = -1,    // Default x position (out of bounds, indicating it's not on the grid)
        .y = -1,    // Default y position (out of bounds, indicating it's not on the grid)
        .hasFired = false
    };
}

void InitObstacles(){ // TODO: Make obstacle generation random and make it compatible with different MAX_OBSTACLES, MIN_OBSTACLES settings.
    obstacles[0] = (Obstacle){.x = 3, .y = 4};
    obstacles[1] = (Obstacle){.x = 6, .y = 2};
    obstacles[2] = (Obstacle){.x = 1, .y = 8};
}

void DrawShip(const Ship* ship, Color color){
    float shipSize = CELL_SIZE * 0.8f;
    float offset = (CELL_SIZE - shipSize) / 2.0f;
    DrawRectangle(
        (int)(ship->x * CELL_SIZE + offset),
        (int)(ship->y * CELL_SIZE + offset),
        (int)shipSize,
        (int)(shipSize),
        color
    );
}

void DrawBullet(const Bullet* bullet, Color color) {
    if (!bullet->hasFired) return;

    float bulletRadius = CELL_SIZE * 0.3f;
    float centerX = bullet->x * CELL_SIZE + CELL_SIZE / 2.0f;
    float centerY = bullet->y * CELL_SIZE + CELL_SIZE / 2.0f;

    DrawCircle((int)centerX, (int)centerY, bulletRadius, color);
}

void UpdateGame(GamePhase *phase, Ship *p1, Ship *p2, Bullet *b1, Bullet *b2){
    switch (*phase)
    {
    case MOVEMENT_PHASE:
        HandleMovementInput(p1);
        HandleMovementInput(p2);
        if(AllPlayersReady()) *phase = INTERRUPTION_PHASE;
        break;
    
    case INTERRUPTION_PHASE:
        HandleShooting(b1);
        HandleShooting(b2);
        if (AllPlayersShot()) *phase = ACTION_PHASE;
        break;
    
    case ACTION_PHASE:
        UpdateShipPosition(p1);
        UpdateShipPosition(p2);
        CheckCollisions(p1);
        CheckCollisions(p2);
        if (ShipsStopped()) *phase = MOVEMENT_PHASE;
        break;
    }
}

void DrawGame(Ship p1, Ship p2, Bullet b1, Bullet b2){
    DrawGrid(GRID_SIZE, CELL_SIZE);
    DrawObstacles();
    DrawShip(&p1, BLUE);
    DrawShip(&p2, RED);
    DrawBullet(&b1, BLACK);
    DrawBullet(&b2, BLACK);
}