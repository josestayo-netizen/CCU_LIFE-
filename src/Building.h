#ifndef BUILDING_H
#define BUILDING_H

#include "raylib.h"

typedef struct {
    Rectangle area;          
    Color color;              
    const char *name;         
    bool isInteractable;      
} Building;

void DrawBuildings(Building *b, int count) {
    for (int i = 0; i < count; i++) {
        
    }
}


int GetNearbyBuilding(Vector2 playerPos, Building *b, int count) {
    for (int i = 0; i < count; i++) {
        Rectangle detectionArea = { 
            b[i].area.x - 20, 
            b[i].area.y - 20, 
            b[i].area.width + 40, 
            b[i].area.height + 40 
        };
        
        if (CheckCollisionCircleRec(playerPos, 20, detectionArea)) {
            return i;
        }
    }
    return -1;
}

#endif