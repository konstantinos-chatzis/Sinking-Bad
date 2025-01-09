#include "final_excecution.h"

bool CheckCollisionHitboxes(const Hitbox hitbox1, const Hitbox hitbox2){
    return CheckCollisionRecs(hitbox1.rect, hitbox2.rect);
}

void FinalExcecution(Player *players){
    for(int i = 0; i < 2; i++){
        // Colision detection
        if (CheckCollisionHitboxes(players[i].ship.hitbox, players[(i+1)%2].bullet.hitbox)){
            printf("Player %d was hit!\n", i);
            players[(i+1)%2].bullet.isActive = false;
        }
    }
}