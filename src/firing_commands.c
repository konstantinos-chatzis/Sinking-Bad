#include "firing_commands.h"

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
}
