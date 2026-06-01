#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <stdbool.h>

// Struktur untuk melacak progress misi harian (DNA 60 Seconds)
typedef struct {
    bool classAttended;    // Apakah sudah ke kelas? [cite: 104]
    bool assignmentsDone;  // Apakah tugas tambahan selesai? [cite: 13]
    bool libraryVisited;   // Apakah sudah ke perpus? [cite: 15]
} DailyMissions;

// Struktur Utama Player (Mahasiswa CCU)
typedef struct {
    // Pergerakan (WASD) [cite: 131]
    Vector2 position;      
    float speed;           

    // Status Utama [cite: 22, 23, 97]
    float energy;          // Stamina fisik (0 - 100) [cite: 24, 98]
    float stress;          // Beban mental (0 - 100) [cite: 25, 99]
    int score;             // Poin akumulasi dari misi [cite: 109]

    // Progres Cerita [cite: 12, 90]
    int currentDay;        // Hari ke-1 sampai ke-5
    DailyMissions missions; // Checklist misi harian 
    
    // Status Visual (Feedback) 
    bool isBlurry;         // Efek layar jika Stress > 80 atau Energy < 20
} Player;

// Fungsi inisialisasi awal game
void InitPlayer(Player *p) {
    p->position = (Vector2){ 400, 300 }; // Contoh posisi awal di Dorm [cite: 16]
    p->speed = 2.0f; 
    p->energy = 50.0f;
    p->stress = 0.0f;
    p->score = 0; 
    p->currentDay = 1; 
    p->isBlurry = false; 
    
    // Reset misi hari pertama [cite: 13, 14, 91, 92]
    p->missions.classAttended = false;
    p->missions.assignmentsDone = false;
    p->missions.libraryVisited = false;
}

#endif