#ifndef UI_H
#define UI_H

#include "raylib.h"

// Tambahkan ini di ui.h
void DrawMissionHUD(int screenWidth, Player p) {
    // 1. Gambar Background Kotak Tugas (Semi-transparan)
    Rectangle taskBox = { screenWidth - 260, 20, 240, 130 };
    DrawRectangleRec(taskBox, Fade(BLACK, 0.5f));
    DrawRectangleLinesEx(taskBox, 2, WHITE);

    DrawText("DAILY MISSIONS:", taskBox.x + 10, taskBox.y + 10, 18, YELLOW);

    // 2. Daftar Tugas
    // Tugas 1: Kuliah
    const char* task1 = p.missions.classAttended ? "[X] Attend Class" : "[ ] Attend Class";
    DrawText(task1, taskBox.x + 10, taskBox.y + 40, 16, p.missions.classAttended ? GREEN : WHITE);

    // Tugas 2: Tugas/Assignment
    const char* task2 = p.missions.assignmentsDone ? "[X] Do Assignments" : "[ ] Do Assignments";
    DrawText(task2, taskBox.x + 10, taskBox.y + 70, 16, p.missions.assignmentsDone ? GREEN : WHITE);

    // Tugas 3: Perpustakaan (Atau ganti jadi push-up?)
    const char* task3 = p.missions.libraryVisited ? "[X] Visit Library" : "[ ] Visit Library";
    DrawText(task3, taskBox.x + 10, taskBox.y + 100, 16, p.missions.libraryVisited ? GREEN : WHITE);
}

// Fungsi untuk menampilkan dialog dengan gaya Visual Novel / Pokemon
void DrawDialogueScene(int screenWidth, int screenHeight, const char* name, const char* text) {
    // 1. Overlay semi-transparan agar dunia di belakang terlihat redup
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.4f));

    // 2. Gambar Portrait Karakter (Sisi Kanan)
    // Sementara pakai kotak, nanti bisa kamu ganti dengan DrawTexture
    Rectangle portraitPos = { screenWidth - 420, screenHeight - 620, 380, 450 };
    DrawRectangleRec(portraitPos, Fade(RAYWHITE, 0.9f)); 
    DrawRectangleLinesEx(portraitPos, 4, DARKGRAY);
    DrawText("CHARACTER\nPORTRAIT", portraitPos.x + 100, portraitPos.y + 200, 25, DARKGRAY);

    // 3. Main Text Box (Bagian Bawah)
    Rectangle box = { 30, screenHeight - 210, screenWidth - 60, 180 };
    
    // Background Putih dengan Border Tebal (Style GBA)
    DrawRectangleRec(box, RAYWHITE);
    DrawRectangleLinesEx(box, 6, MAROON); // Border Luar
    DrawRectangleLinesEx((Rectangle){box.x + 6, box.y + 6, box.width - 12, box.height - 12}, 2, BLACK); // Border Dalam

    // 4. Label Nama Karakter
    DrawRectangle(box.x + 30, box.y - 30, 200, 40, MAROON);
    DrawText(name, box.x + 50, box.y - 22, 22, WHITE);

    // 5. Isi Teks Dialog
    // Menggunakan font size 30 agar mudah dibaca seperti di gambar referensimu
    DrawText(text, box.x + 40, box.y + 45, 30, DARKBLUE);

    // Indikator "Next" (Segitiga kecil yang berkedip)
    if ((int)(GetTime() * 3) % 2 == 0) {
        DrawTriangle((Vector2){box.x + box.width - 50, box.y + box.height - 45},
                     (Vector2){box.x + box.width - 20, box.y + box.height - 45},
                     (Vector2){box.x + box.width - 35, box.y + box.height - 25}, MAROON);
    }
}

// Fungsi dasar untuk menggambar kotak pop-up di tengah layar
void DrawPopUpBase(int screenWidth, int screenHeight, const char* title) {
    // 1. Overlay gelap agar game di belakang terlihat redup
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    
    // 2. Kotak Putih di tengah
    Rectangle box = { screenWidth/2 - 250, screenHeight/2 - 150, 500, 300 };
    DrawRectangleRec(box, RAYWHITE);
    DrawRectangleLinesEx(box, 3, DARKGRAY);

    // 3. Judul Menu
    DrawText(title, box.x + 20, box.y + 20, 25, BLACK);
    DrawLine(box.x + 20, box.y + 55, box.x + 480, box.y + 55, LIGHTGRAY);
}

// Menu Spesifik Cafeteria
void DrawCafeteriaMenu(int screenWidth, int screenHeight) {
    DrawPopUpBase(screenWidth, screenHeight, "CAMPUS CAFETERIA");
    int startX = screenWidth/2 - 210;
    int startY = screenHeight/2 - 60;

    DrawText("1. Grilled Chicken Breast (Energy +30)", startX, startY, 20, DARKGREEN);
    DrawText("2. Boiled Egg Whites (Energy +15)", startX, startY + 40, 20, DARKGREEN);
    DrawText("3. Mineral Water (Stress -5)", startX, startY + 80, 20, BLUE);
    
    DrawText("Press [ESC] to Exit", startX, startY + 160, 15, GRAY);
}

// Menu Spesifik CSIE
void DrawCSIEMenu(int screenWidth, int screenHeight) {
    DrawPopUpBase(screenWidth, screenHeight, "CSIE DEPARTMENT");
    int startX = screenWidth/2 - 210;
    int startY = screenHeight/2 - 60;

    DrawText("1. Attend Calculus Class", startX, startY, 20, DARKBLUE);
    DrawText("2. Submit Assignment", startX, startY + 40, 20, DARKBLUE);
    
    DrawText("Press [ESC] to Exit", startX, startY + 160, 15, GRAY);
}

#endif