#include <game.h>
#include <phase_movement_commands.h>

const float minSpeed = 1.0f;
const float maxSpeed = 10.0f;

// Global variables for textures
Texture2D sliderBackground;
Texture2D sliderFill;
Texture2D sliderKnob;

// Function to load textures (call this at the beginning of your program)
void LoadSliderTextures() {
    sliderBackground = LoadTexture("assets/sprites/slider_bar_background.png");
    sliderFill = LoadTexture("assets/sprites/slider_bar_fill.png");
    sliderKnob = LoadTexture("assets/sprites/slider_bar_knob.png");
}

// Function to unload textures (call this at the end of your program)
void UnloadSliderTextures() {
    UnloadTexture(sliderBackground);
    UnloadTexture(sliderFill);
    UnloadTexture(sliderKnob);
}

void ShipDirectionInput(Player (*players)[2], int* currentPlayerIndex){
    Player* currentPlayer = &(*players)[*currentPlayerIndex];
    if (!currentPlayer->hasSelectedDirection){
        Vector2 mousePosition = GetMousePosition();
        currentPlayer->ship.rotation = atan2(currentPlayer->ship.position.y - mousePosition.y, currentPlayer->ship.position.x - mousePosition.x)*RAD2DEG+90;

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentPlayer->hasSelectedDirection = true;
            *currentPlayerIndex = (*currentPlayerIndex + 1) % 2;
        }
    }
}

void ShipDirectionDrawing(Player (*players)[2], int* currentPlayerIndex){ // This function goes in UpdateGameDraw() in game.c | (*players)[2] is a pointer to the entire array
    if (*currentPlayerIndex == 0 && !(*players)[0].hasSelectedDirection) {
        DrawText("Player 1", 50, 50, 50, BLUE);
        DrawText(", select your ship's direction!", 255, 50, 50, GRAY); // 205 units more that the previous.
    } 
    else if (*currentPlayerIndex == 1 && !(*players)[1].hasSelectedDirection) {
        DrawText("Player 2", 50, 50, 50, RED);
        DrawText(", select your ship's direction!", 270, 50, 50, GRAY); // 220 units more that the previous.
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
    }
}

void ShipSpeedInputDrawing(Player (*players)[2], int* currentPlayerIndex){
    Player* currentPlayer = &(*players)[*currentPlayerIndex];
    if(currentPlayer->hasSelectedDirection && !currentPlayer->hasSelectedSpeed){
        // Custom vertical slider logic
        Rectangle sliderBounds = {100, 200, 40, 400};
        int mouseY = GetMouseY();
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), sliderBounds)) {
            currentPlayer->ship.speed = minSpeed + (maxSpeed - minSpeed) * ((float)(sliderBounds.y + sliderBounds.height - mouseY) / sliderBounds.height);
            if (currentPlayer->ship.speed < minSpeed) currentPlayer->ship.speed = minSpeed;
            if (currentPlayer->ship.speed > maxSpeed) currentPlayer->ship.speed = maxSpeed;
        }

        float sliderFillHeight = ((float)(currentPlayer->ship.speed - minSpeed) / (maxSpeed - minSpeed)) * sliderBounds.height;

        // Draw custom slider background
        DrawTexturePro(sliderBackground, (Rectangle){0, 0, sliderBackground.width, sliderBackground.height}, sliderBounds, (Vector2){0, 0}, 0.0f, WHITE);

        // Draw custom slider fill with clipping
        Rectangle fillBounds = {sliderBounds.x, sliderBounds.y + sliderBounds.height - sliderFillHeight, sliderBounds.width, sliderFillHeight};
        BeginScissorMode(fillBounds.x, fillBounds.y, fillBounds.width, fillBounds.height);
        DrawTexturePro(sliderFill, (Rectangle){0, 0, sliderFill.width, sliderFill.height}, sliderBounds, (Vector2){0, 0}, 0.0f, WHITE);
        EndScissorMode();

        // Draw custom slider knob
        float knobY = sliderBounds.y + sliderBounds.height - sliderFillHeight - sliderKnob.height / 2;
        if (knobY < sliderBounds.y) knobY = sliderBounds.y;
        if (knobY > sliderBounds.y + sliderBounds.height - sliderKnob.height -25.0) knobY = sliderBounds.y + sliderBounds.height - sliderKnob.height - 25.0;
        Rectangle knobBounds = {sliderBounds.x + (sliderBounds.width - sliderKnob.width) / 8 -4.0, knobY, sliderBounds.width, sliderKnob.height * (sliderBounds.width / sliderKnob.width)};
        DrawTexturePro(sliderKnob, (Rectangle){0, 0, sliderKnob.width, sliderKnob.height}, knobBounds, (Vector2){0, 0}, 0.0f, WHITE);

        // Draw speed text
        DrawText(TextFormat("Speed: %.1f", currentPlayer->ship.speed), sliderBounds.x + sliderBounds.width + 10, sliderBounds.y, 20, DARKGRAY);
    }
}

// Vector2 centerOfShip = ...