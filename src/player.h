#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    bool classAttended;    
    bool assignmentsDone; 
    bool libraryVisited;  
} DailyMissions;

typedef struct {
    Vector2 position;      
    float speed;           
    float energy;          
    float stress;          
    int score;            
    int currentDay;       
    DailyMissions missions; 
    bool isBlurry;         
} Player;

void InitPlayer(Player *p) {
    p->position = (Vector2){ 400, 300 }; 
    p->speed = 2.0f; 
    p->energy = 50.0f;
    p->stress = 0.0f;
    p->score = 0; 
    p->currentDay = 1; 
    p->isBlurry = false; 
    p->missions.classAttended = false;
    p->missions.assignmentsDone = false;
    p->missions.libraryVisited = false;
}

#endif