#include <game.h>

Player players[2] = {
    {1, {{0, 0}, 0.0f, false, BLUE}, BLUE},
    {2, {{0, 0}, 0.0f, false, RED}, RED}
};

int currentPlayerIndex = 0; // 0 for player 1 (BLUE), 1 for player 2 (RED)
GamePhase currentPhase = SHIP_DEPLOYMENT;

Vector2 ship1Position = {0, 0};
bool drawShip1 = false;
float ship1Rotation = 0.0f;
bool ship1Deployed = false;

Vector2 ship2Position = {0, 0};
bool drawShip2 = false;
float ship2Rotation = 0.0f;
bool ship2Deployed = false;

// 
void ShipDeploymentInput(){ // This function goes in the main game loop before BeginDrawing(); 
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        Player* currentPlayer = &players[currentPlayerIndex];
        currentPlayer->ship.position = GetMousePosition();
        currentPlayer->ship.rotation = (float)GetRandomValue(0, 360);
        currentPlayer->ship.deployed = true;

        currentPlayerIndex = (currentPlayerIndex + 1) % 2;
    }
}

// 
void ShipDeploymentDrawing(){ // This function goes in the main game loop after BeginDrawing();
    if (currentPlayerIndex == 0 && !players[0].ship.deployed) {
        DrawText("Player 1", 50, 50, 50, BLUE);
        DrawText(", deploy your ship!", 255, 50, 50, GRAY); // 205 units more that the previous.
    } 
    else if (currentPlayerIndex == 1 && !players[1].ship.deployed) {
        DrawText("Player 2", 50, 50, 50, RED);
        DrawText(", deploy your ship!", 270, 50, 50, GRAY); // 220 units more that the previous.
    }

    for (int i = 0; i < 2; i++){
        if (players[i].ship.deployed){
            DrawRectanglePro(
                (Rectangle){players[i].ship.position.x, players[i].ship.position.y, 100, 50},
                (Vector2){100 / 2, 50 / 2},
                players[i].ship.rotation,
                players[i].color
            );
        }
    }
}

void UpdateGameInput(){
    if (currentPhase == SHIP_DEPLOYMENT) ShipDeploymentInput(); 
}

void UpdateGameDraw(){
    ShipDeploymentDrawing();
}