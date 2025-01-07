#include "movement_commands.h"

// Global variables for textures
Texture2D sliderBackground;
Texture2D sliderFill;
Texture2D sliderKnob;

// Global variables for button textures
Texture2D buttonNormal;
Texture2D buttonHover;
Texture2D buttonClick;

float selectedSpeed = 1.0f;

// Function to load textures (call this at the beginning of your program)
void LoadSpeedSelectionTextures() {
    // Slider textures
    sliderBackground = LoadTexture("assets/sprites/slider_bar_background.png");
    sliderFill = LoadTexture("assets/sprites/slider_bar_fill.png");
    sliderKnob = LoadTexture("assets/sprites/slider_bar_knob.png");

    // Button textures
    buttonNormal = LoadTexture("assets/sprites/button_normal.png");
    buttonHover = LoadTexture("assets/sprites/button_hover.png");
    buttonClick = LoadTexture("assets/sprites/button_clicked.png");
}

// Function to unload textures (call this at the end of your program)
void UnloadSpeedSelectionTextures() {
    // Slider textures
    UnloadTexture(sliderBackground);
    UnloadTexture(sliderFill);
    UnloadTexture(sliderKnob);

    // Button textures
    UnloadTexture(buttonNormal);
    UnloadTexture(buttonHover);
    UnloadTexture(buttonClick);
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
}

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
        sliderBounds.x + sliderBounds.width + 10,
        sliderBounds.y,
        20,
        DARKGRAY
    );
}

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

void ShipSpeedInputDrawing(Player (*players)[2], int* currentPlayerIndex, float minSpeed, float maxSpeed) {
    Player* currentPlayer = &(*players)[*currentPlayerIndex];

    // Display instructions for the current player
    if (*currentPlayerIndex == 0 && !(*players)[0].hasSelectedSpeed) {
        DrawText("Player 1", 50, 50, 50, BLUE);
        DrawText(", select your ship's speed!", 255, 50, 50, GRAY);
    } else if (*currentPlayerIndex == 1 && !(*players)[1].hasSelectedSpeed) {
        DrawText("Player 2", 50, 50, 50, RED);
        DrawText(", select your ship's speed!", 270, 50, 50, GRAY);
    }

    // Slider logic
    Rectangle sliderBounds = {100, 200, 40, 400};
    HandleShipSpeedInput(currentPlayer, minSpeed, maxSpeed, sliderBounds);
    DrawShipSpeedSlider(currentPlayer, minSpeed, maxSpeed, sliderBounds);

    // Draw confirmation button
    static bool buttonClicked = false;
    Rectangle buttonBounds = {
        sliderBounds.x,
        sliderBounds.y + sliderBounds.height + 20,
        buttonNormal.width * 5,
        buttonNormal.height * 5
    };
    Vector2 mousePosition = GetMousePosition();

    if (CheckCollisionPointRec(mousePosition, buttonBounds)) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            DrawTexturePro(
                buttonClick,
                (Rectangle){0, 0, buttonClick.width, buttonClick.height},
                buttonBounds,
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
            buttonClicked = true; 
        } else if (buttonClicked && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            // Button action is triggered only when the mouse is released over the button
            currentPlayer->ship.speed = selectedSpeed;
            currentPlayer->hasSelectedSpeed = true;
            *currentPlayerIndex = (*currentPlayerIndex + 1) % 2;
            selectedSpeed = 1.0f; // Reset the selected speed
            buttonClicked = false;

        } else {
            DrawTexturePro(
                buttonHover,
                (Rectangle){0, 0, buttonHover.width, buttonHover.height},
                buttonBounds,
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
        }
    } else {
        DrawTexturePro(
            buttonNormal,
            (Rectangle){0, 0, buttonNormal.width, buttonNormal.height},
            buttonBounds,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            buttonClicked = false; // Reset the button state if the mouse is released outside
        }
    }
}