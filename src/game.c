#include <game.h>
#include <stdio.h>

Player players[2] = {
    {1, {{0, 0}, 0.0f, false, BLUE}, BLUE},
    {2, {{0, 0}, 0.0f, false, RED}, RED}
};

int currentPlayerIndex = 0; // 0 for player 1 (BLUE), 1 for player 2 (RED)
GamePhase currentPhase = SHIP_DEPLOYMENT;

// 
void ShipDeploymentInput(){ // This function goes in UpdateGameInput() in game.c
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        Player* currentPlayer = &players[currentPlayerIndex];
        currentPlayer->ship.position = GetMousePosition();
        currentPlayer->ship.rotation = (float)GetRandomValue(0, 360);
        currentPlayer->ship.deployed = true;

        currentPlayerIndex = (currentPlayerIndex + 1) % 2;
    }
}

// 
void ShipDeploymentDrawing(){ // This function goes in UpdateGameDraw() in game.c
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
            DrawTexturePro(
                players[i].ship.texture,
                (Rectangle){0, 0, players[i].ship.texture.width, players[i].ship.texture.height},
                (Rectangle){players[i].ship.position.x, players[i].ship.position.y, players[i].ship.texture.width*5, players[i].ship.texture.height*5},
                (Vector2){(players[i].ship.texture.width*5)/2.0f, (players[i].ship.texture.height*5)/2.0f},
                players[i].ship.rotation,
                WHITE
            );
        }
    }
}

void UpdateGameInput(){ // This function goes in the main game loop before BeginDrawing(); in main.c
    if (currentPhase == SHIP_DEPLOYMENT) ShipDeploymentInput();
}

void UpdateGameDraw(){ // This function goes in the main game loop after BeginDrawing(); in main.c
    if (currentPhase == SHIP_DEPLOYMENT) ShipDeploymentDrawing();
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
        players[i].ship.texture = LoadShipTexture(players[i].ship.color);
    }
}

void UnloadGameTextures(){
    for(int i = 0; i < 2;i++){
        UnloadTexture(players[i].ship.texture);
    }
}