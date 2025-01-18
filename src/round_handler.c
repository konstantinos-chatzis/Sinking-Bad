#include "round_handler.h"

// Checks collisions between players and enemy projectiles and players and bombs
void CheckCollisions(Player *players, Bomb *bombs) {
    for(int i = 0; i < 2; i++) {
        // Colision detection
        if (CheckCollisionHitboxes(&players[i].ship.hitbox, &players[(i+1)%2].projectile.hitbox)) {
            players[i].isHit = true;
            players[(i+1)%2].projectile.isActive = false;
        }

        for (int j = 0; j < 4; j++)
        {
            if(CheckCollisionHitboxes(&players[i].ship.hitbox, &bombs[j].hitbox)) {
                players[i].isHit = true;
            }
        }
        
    }
}

// Checks is a player is hit by a projectile or a bomb and updates the score
void CheckRoundWinCondition(Player *players, Bomb *bombs) {
    CheckCollisions(players, bombs);
    
    if(!isRoundOver) {
        if(players[0].isHit) {
            PlaySound(explosionSound);
            players[1].score++;
            isRoundOver = true;
        }
        else if(players[1].isHit) {
            PlaySound(explosionSound);
            players[0].score++;
            isRoundOver = true;
        }
    }
}