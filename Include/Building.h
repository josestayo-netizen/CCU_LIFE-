#ifndef BUILDING_H
#define BUILDING_H

#include "raylib.h"

// 1. Definisi Struktur Gedung
typedef struct {
    Rectangle area;           // Posisi dan ukuran di map
    Color color;              // Warna (sementara sebelum ada sprite)
    const char *name;         // Nama gedung (Dorm, CSIE, dll)
    bool isInteractable;      // Apakah bisa dimasuki?
} Building;

// 2. Fungsi untuk menggambar semua gedung sekaligus
// Ini akan merapikan loop di main.c
void DrawBuildings(Building *b, int count) {
    for (int i = 0; i < count; i++) {
        DrawRectangleRec(b[i].area, b[i].color);
        // Gambar nama gedung sedikit di atas atau di dalam gedung
        DrawText(b[i].name, b[i].area.x + 5, b[i].area.y + 5, 18, BLACK);
    }
}

// 3. Fungsi untuk mengecek gedung mana yang paling dekat dengan Andreas
// Mengembalikan index gedung (-1 jika tidak ada yang dekat)
int GetNearbyBuilding(Vector2 playerPos, Building *b, int count) {
    for (int i = 0; i < count; i++) {
        // Area deteksi sedikit lebih besar dari gedung itu sendiri
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