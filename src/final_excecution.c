#include "final_excecution.h"

// Project polygon corners onto an axis and find the minimum and maximum projections
void ProjectPolygon(const Vector2* corners, int numCorners, Vector2 axis, float* min, float* max) {
    *min = FLT_MAX;
    *max = -FLT_MAX;

    for (int i = 0; i < numCorners; i++) {
        // Projection of the corner onto the axis
        float projection = corners[i].x * axis.x + corners[i].y * axis.y; // Dot product
        if (projection < *min) *min = projection;
        if (projection > *max) *max = projection;
    }
}

// Check for overlap between two projected ranges
bool CheckOverlap(float minA, float maxA, float minB, float maxB) {
    return !(maxA < minB || maxB < minA); // No gap means overlap
}

// Main SAT collision detection function
bool CheckSATCollision(const Hitbox* hitboxA, const Hitbox* hitboxB) {
    // Get the corners of both hitboxes
    Vector2 cornersA[4], cornersB[4];
    GetHitboxCorners(hitboxA, cornersA);
    GetHitboxCorners(hitboxB, cornersB);

    // Check for overlap along each edge of both polygons
    Vector2 axes[4];
    
    // Get the edges and the axes perpendicular to them
    for (int i = 0; i < 4; i++) {
        // Edge for hitbox A
        Vector2 edgeA = (i == 3) ? Vector2Subtract(cornersA[0], cornersA[3]) : Vector2Subtract(cornersA[i + 1], cornersA[i]);
        axes[i] = (Vector2){-edgeA.y, edgeA.x};  // Perpendicular to the edge
    }

    // Check for overlap along each axis
    for (int i = 0; i < 4; i++) {
        // Project hitbox A and B onto the axis
        float minA, maxA, minB, maxB;
        ProjectPolygon(cornersA, 4, axes[i], &minA, &maxA);
        ProjectPolygon(cornersB, 4, axes[i], &minB, &maxB);

        // Check if there's no overlap
        if (!CheckOverlap(minA, maxA, minB, maxB)) {
            return false;  // No collision if there's no overlap on this axis
        }
    }

    // If no separating axis is found, there is a collision
    return true;
}