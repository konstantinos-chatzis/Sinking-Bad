#include "movement_directions.h"

// Global variables for textures
Texture2D sliderBackground;
Texture2D sliderFill;
Texture2D sliderKnob;

float selectedSpeed = 1.0f;

// Function to load speed selection textures
void LoadSpeedSelectionTextures() {
    // Slider textures
    sliderBackground = LoadTexture("assets/sprites/slider_bar_background.png");
    sliderFill = LoadTexture("assets/sprites/slider_bar_fill.png");
    sliderKnob = LoadTexture("assets/sprites/slider_bar_knob.png");
}

// Function to unload speed selection textures
void UnloadSpeedSelectionTextures() {
    // Slider textures
    UnloadTexture(sliderBackground);
    UnloadTexture(sliderFill);
    UnloadTexture(sliderKnob);
}

// Get mouse inpute for the ship's direction
void ShipDirectionInput(Player *players, int* currentPlayerIndex) {
    Player* currentPlayer = &players[*currentPlayerIndex];
    if (!currentPlayer->hasSelectedDirection){
        Vector2 mousePosition = GetMousePosition();
        currentPlayer->ship.rotation = atan2(currentPlayer->ship.position.y - mousePosition.y, currentPlayer->ship.position.x - mousePosition.x)*RAD2DEG+90;

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            currentPlayer->hasSelectedDirection = true;
            *currentPlayerIndex = (*currentPlayerIndex + 1) % 2;
        }
    }
}

// Draw ship direction suggestive text
void ShipDirectionDrawing(Player *players, int* currentPlayerIndex) { // This function goes in UpdateGameDraw() in game.c | (*players)[2] is a pointer to the entire array
    if (*currentPlayerIndex == 0 && !players[0].hasSelectedDirection) {
        DrawText("Player 1", 50, 40, 50, BLUE);
        DrawText(", select your ship's direction!", 255, 40, 50, RAYWHITE); // 205 units more that the previous.
    } 
    else if (*currentPlayerIndex == 1 && !players[1].hasSelectedDirection) {
        DrawText("Player 2", 50, 40, 50, RED);
        DrawText(", select your ship's direction!", 270, 40, 50, RAYWHITE); // 220 units more that the previous.
    }
}

// Draw ship speed selection slider
void DrawShipSpeedSlider(Player* currentPlayer, float minSpeed, float maxSpeed, Rectangle sliderBounds) {
    // Calculate the slider fill height based on the current speed
    float sliderFillHeight = ((selectedSpeed - minSpeed) / (maxSpeed - minSpeed)) * sliderBounds.height;

    // Draw custom slider background
    DrawTexturePro(
        sliderBackground,
        (Rectangle){0, 0, sliderBackground.width, sliderBackground.height},
        sliderBounds,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );

    // Draw custom slider fill with clipping
    Rectangle fillBounds = {
        sliderBounds.x,
        sliderBounds.y + sliderBounds.height - sliderFillHeight,
        sliderBounds.width,
        sliderFillHeight
    };
    BeginScissorMode(fillBounds.x, fillBounds.y, fillBounds.width, fillBounds.height);
    DrawTexturePro(
        sliderFill,
        (Rectangle){0, 0, sliderFill.width, sliderFill.height},
        sliderBounds,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
    EndScissorMode();

    // Calculate the knob's position
    float knobY = sliderBounds.y + sliderBounds.height - sliderFillHeight - 25.0f; // Logical offset from the slider top
    if (knobY < sliderBounds.y) knobY = sliderBounds.y; // Keep within top bounds
    if (knobY > sliderBounds.y + sliderBounds.height - 50.0f) knobY = sliderBounds.y + sliderBounds.height - 50.0f; // Keep within bottom bounds

    // Adjust knob size
    float knobScaleFactor = 1.5f; // Increase the knob size
    float knobWidth = sliderBounds.width * knobScaleFactor;
    float knobHeight = sliderKnob.height * (knobWidth / sliderKnob.width);

    // Center the knob horizontally
    Rectangle knobBounds = {
        sliderBounds.x + (sliderBounds.width - knobWidth) / 2.0f,
        knobY,
        knobWidth,
        knobHeight
    };

    // Draw the slider knob
    DrawTexturePro(
        sliderKnob,
        (Rectangle){0, 0, sliderKnob.width, sliderKnob.height},
        knobBounds,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );

    // Draw speed text
    DrawText(
        TextFormat("Speed: %.1f", selectedSpeed),
        (sliderBounds.x + sliderBounds.width + 10) -75,
        (sliderBounds.y) -30,
        20,
        DARKGRAY
    );
}

// Handle slider-mouse position dynamics
void HandleShipSpeedInput(Player* player, float minSpeed, float maxSpeed, Rectangle sliderBounds) {
    if (!player->hasSelectedSpeed) {
        Vector2 mousePosition = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, sliderBounds)) {
            float mouseOffset = sliderBounds.y + sliderBounds.height - mousePosition.y;
            selectedSpeed = minSpeed + (maxSpeed - minSpeed) * (mouseOffset / sliderBounds.height);

            // Clamp speed within range
            if (selectedSpeed < minSpeed) selectedSpeed = minSpeed;
            if (selectedSpeed > maxSpeed) selectedSpeed = maxSpeed;
        }
    }
}

// Calls the above input functions, draw speed selection suggestive text and draw selection button
void ShipSpeedInputDrawing(Player *players, int* currentPlayerIndex, float minSpeed, float maxSpeed, Button *selectButton) {
    Player* currentPlayer = &players[*currentPlayerIndex];

    // Display instructions for the current player
    if (*currentPlayerIndex == 0 && !players[0].hasSelectedSpeed) {
        DrawText("Player 1", 50, 40, 50, BLUE);
        DrawText(", select your ship's speed!", 255, 40, 50, RAYWHITE);
    } else if (*currentPlayerIndex == 1 && !players[1].hasSelectedSpeed) {
        DrawText("Player 2", 50, 40, 50, RED);
        DrawText(", select your ship's speed!", 270, 40, 50, RAYWHITE);
    }

    // Slider logic
    Rectangle sliderBounds = {45, 300, 40, 400};
    HandleShipSpeedInput(currentPlayer, minSpeed, maxSpeed, sliderBounds);
    DrawShipSpeedSlider(currentPlayer, minSpeed, maxSpeed, sliderBounds);

    if (UpdateAndDrawButton(selectButton)) {
        currentPlayer->ship.speed = selectedSpeed;
        currentPlayer->hasSelectedSpeed = true;
        *currentPlayerIndex = (*currentPlayerIndex + 1) % 2; // Move to the next player
        selectedSpeed = 1.0f; // Reset the selected speed

        // Reset the button state for the new player
        selectButton->isClicked = false;
    }
}