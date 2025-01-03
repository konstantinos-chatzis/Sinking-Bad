#include <game.h>
#include <phase_ship_deployment.h>

// 
void ShipDeploymentInput(Player (*players)[2], int* currentPlayerIndex){ // This function goes in UpdateGameInput() in game.c
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        Player* currentPlayer = &(*players)[*currentPlayerIndex];
        currentPlayer->ship.position = GetMousePosition();
        currentPlayer->ship.rotation = (float)GetRandomValue(0, 360);
        currentPlayer->hasDeployed = true;

        *currentPlayerIndex = (*currentPlayerIndex + 1) % 2;
    }
}

// 
void ShipDeploymentDrawing(Player (*players)[2], int* currentPlayerIndex){ // This function goes in UpdateGameDraw() in game.c | (*players)[2] is a pointer to the entire array
    if (*currentPlayerIndex == 0 && !(*players)[0].hasDeployed) {
        DrawText("Player 1", 50, 50, 50, BLUE);
        DrawText(", deploy your ship!", 255, 50, 50, GRAY); // 205 units more that the previous.
    } 
    else if (*currentPlayerIndex == 1 && !(*players)[1].hasDeployed) {
        DrawText("Player 2", 50, 50, 50, RED);
        DrawText(", deploy your ship!", 270, 50, 50, GRAY); // 220 units more that the previous.
    }

    for (int i = 0; i < 2; i++){
        if ((*players)[i].hasDeployed){
            DrawTexturePro(
                (*players)[i].ship.texture,
                (Rectangle){0, 0, (*players)[i].ship.texture.width, (*players)[i].ship.texture.height},
                (Rectangle){(*players)[i].ship.position.x, (*players)[i].ship.position.y, (*players)[i].ship.texture.width*5, (*players)[i].ship.texture.height*5},
                (Vector2){((*players)[i].ship.texture.width*5)/2.0f, ((*players)[i].ship.texture.height*5)/2.0f},
                (*players)[i].ship.rotation,
                WHITE
            );
        }
    }
}