#include "title_screen.h"

void DrawTitleScreen(GamePhase *currentPhase, Button *playButton, Button *loadButton, Button *quitButton) {

    // Title
    DrawText("Si", 590, 100, 100, DARKGREEN);
    DrawText("nking", 680, 100, 100, DARKBLUE);
    DrawText("Ba", 990, 100, 100, DARKGREEN);
    DrawText("d", 1120, 100, 100, DARKBLUE);

    // Buttons

    // NEW
    if (UpdateAndDrawButton(playButton)) {
        *currentPhase = SHIP_DEPLOYMENT;
    }
    if (UpdateAndDrawButton(loadButton)) {
        LoadRestoreGame();
    }
    if (UpdateAndDrawButton(quitButton)) {
        CloseWindow(); // Trigger action for Quit
    }
}
