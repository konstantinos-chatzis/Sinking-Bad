#include "firing_commands.h"

const float bombScale = 1.5f;

void FiringCommandsInput(Player (*players)[2], int* currentPlayerIndex){
    Player* currentPlayer = &(*players)[*currentPlayerIndex];
    if(!currentPlayer->hasFired){
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentPlayer->bomb.position = GetMousePosition();
            currentPlayer->hasFired = true;
            *currentPlayerIndex = (*currentPlayerIndex + 1) % 2;
        }
    }
}

void FiringCommandsDrawing(Player (*players)[2], int* currentPlayerIndex){
    if (*currentPlayerIndex == 0 && !(*players)[0].hasFired) {
        DrawText("Player 1", 50, 50, 50, BLUE);
        DrawText(", deploy your bomb!", 255, 50, 50, GRAY); // 205 units more that the previous.
    } 
    else if (*currentPlayerIndex == 1 && !(*players)[1].hasFired) {
        DrawText("Player 2", 50, 50, 50, RED);
        DrawText(", deploy your bomb!!", 270, 50, 50, GRAY); // 220 units more that the previous.
    }

    for (int i = 0; i < 2; i++){
        DrawTexturePro(
            (*players)[i].ship.texture,
            (Rectangle){0, 0, (*players)[i].ship.texture.width, (*players)[i].ship.texture.height},
            (Rectangle){(*players)[i].ship.position.x, (*players)[i].ship.position.y, (*players)[i].ship.texture.width*5, (*players)[i].ship.texture.height*5},
            (Vector2){((*players)[i].ship.texture.width*5)/2.0f, ((*players)[i].ship.texture.height*5)/2.0f},
            (*players)[i].ship.rotation,
            WHITE
        );

        if((*players)[i].hasFired){
            DrawTexturePro(
                (*players)[i].bomb.texture,
                (Rectangle){0, 0, (*players)[i].bomb.texture.width, (*players)[i].bomb.texture.height},
                (Rectangle){(*players)[i].bomb.position.x, (*players)[i].bomb.position.y, (*players)[i].bomb.texture.width*bombScale, (*players)[i].bomb.texture.height*bombScale},
                (Vector2){((*players)[i].bomb.texture.width*bombScale)/2.0f, ((*players)[i].bomb.texture.height*bombScale)/2.0f},
                0.0f,
                WHITE
            );
        }
    }
}
