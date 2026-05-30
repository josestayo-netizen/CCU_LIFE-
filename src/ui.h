#ifndef UI_H
#define UI_H



#include "raylib.h"
#include "player.h"

// --- 1. HUD DAFTAR TUGAS HARIAN ---
void DrawMissionHUD(int screenWidth, Player p, int day) {
    // Gambar Background Kotak Tugas (Semi-transparan)
    Rectangle taskBox = { screenWidth - 260, 20, 240, 130 };
    DrawRectangleRec(taskBox, Fade(BLACK, 0.5f));
    DrawRectangleLinesEx(taskBox, 2, WHITE);

    DrawText(TextFormat("DAY %d MISSIONS:", day), taskBox.x + 10, taskBox.y + 10, 18, YELLOW);
    // Tugas 1: Kuliah
    const char* task1 = p.missions.classAttended ? "[X] Attend Calculus Class" : "[ ] Attend Calculus Class";
    DrawText(task1, taskBox.x + 10, taskBox.y + 40, 16, p.missions.classAttended ? GREEN : WHITE);

    if (day == 1) {
        // Di Hari 1, assignmentsDone mewakili Kelas Inggris
        const char* task2 = p.missions.assignmentsDone ? "[X] Attend English Class" : "[ ] Attend English Class";
        DrawText(task2, taskBox.x + 10, taskBox.y + 70, 16, p.missions.assignmentsDone ? GREEN : WHITE);
    } 
    else {
        // Di Hari 2, assignmentsDone mewakili Kelas Mandarin
        const char* task2 = p.missions.assignmentsDone ? "[X] Attend Mandarin Class" : "[ ] Attend Mandarin Class";
        DrawText(task2, taskBox.x + 10, taskBox.y + 70, 16, p.missions.assignmentsDone ? GREEN : WHITE);
    }
}

// --- 2. DIALOGUE SYSTEM (VISUAL NOVEL STYLE) ---
void DrawDialogueScene(int screenWidth, int screenHeight, const char* name, const char* text) {
    // Overlay semi-transparan agar dunia di belakang terlihat redup
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.4f));

    // Gambar Portrait Karakter (Sisi Kanan)
    Rectangle portraitPos = { screenWidth - 420, screenHeight - 620, 380, 450 };
    DrawRectangleRec(portraitPos, Fade(RAYWHITE, 0.9f)); 
    DrawRectangleLinesEx(portraitPos, 4, DARKGRAY);
    DrawText("CHARACTER\nPORTRAIT", portraitPos.x + 100, portraitPos.y + 200, 25, DARKGRAY);

    // Main Text Box (Bagian Bawah)
    Rectangle box = { 30, screenHeight - 210, screenWidth - 60, 180 };
    
    // Background Putih dengan Border Tebal (Style GBA)
    DrawRectangleRec(box, RAYWHITE);
    DrawRectangleLinesEx(box, 6, MAROON); 
    DrawRectangleLinesEx((Rectangle){box.x + 6, box.y + 6, box.width - 12, box.height - 12}, 2, BLACK); 

    // Label Nama Karakter
    DrawRectangle(box.x + 30, box.y - 30, 200, 40, MAROON);
    DrawText(name, box.x + 50, box.y - 22, 22, WHITE);

    // Isi Teks Dialog
    DrawText(text, box.x + 40, box.y + 45, 30, DARKBLUE);

    // Indikator "Next" Segitiga Berkedip
    if ((int)(GetTime() * 3) % 2 == 0) {
        DrawTriangle((Vector2){box.x + box.width - 50, box.y + box.height - 45},
                     (Vector2){box.x + box.width - 20, box.y + box.height - 45},
                     (Vector2){box.x + box.width - 35, box.y + box.height - 25}, MAROON);
    }
}

// --- 3. BASE POP-UP MENU ---
void DrawPopUpBase(int screenWidth, int screenHeight, const char* title) {
    // Overlay gelap agar game di belakang terlihat redup
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    
    // Kotak Putih di tengah
    Rectangle box = { screenWidth/2 - 250, screenHeight/2 - 150, 500, 300 };
    DrawRectangleRec(box, RAYWHITE);
    DrawRectangleLinesEx(box, 3, DARKGRAY);

    // Judul Menu
    DrawText(title, box.x + 20, box.y + 20, 25, BLACK);
    DrawLine(box.x + 20, box.y + 55, box.x + 480, box.y + 55, LIGHTGRAY);
}

// --- 4. MENU GEDUNG-GEDUNG ---

void DrawLanguageCenterMenu(int screenWidth, int screenHeight, int day) {
   DrawPopUpBase(screenWidth, screenHeight, "LANGUAGE CENTER");
    int startX = screenWidth/2 - 210;
    int startY = screenHeight/2 - 60;

    if (day == 1) {
        // Hari 1 cuma menampilkan kuis Inggris sesuai logika main.c
        DrawText("2. Study English Word Scramble", startX, startY, 20, DARKBLUE);
    } 
    else {
        // Hari 2 ke atas cuma menampilkan kuis Mandarin sesuai logika main.c
        DrawText("1. Study Mandarin Pinyin", startX, startY, 20, DARKBLUE);
    }
    
    DrawText("Press [BACKSPACE] to Exit", startX, startY + 160, 15, GRAY);
}

// Menu Cafeteria
void DrawCafeteriaMenu(int screenWidth, int screenHeight) {
    DrawPopUpBase(screenWidth, screenHeight, "CAMPUS CAFETERIA");
    int startX = screenWidth/2 - 210;
    int startY = screenHeight/2 - 60;

    DrawText("1. Grilled Chicken Breast (Energy +30)", startX, startY, 20, DARKGREEN);
    DrawText("2. Boiled Egg Whites (Energy +15)", startX, startY + 40, 20, DARKGREEN);
    DrawText("3. Mineral Water (Stress -5)", startX, startY + 80, 20, BLUE);
    
    DrawText("Press [BACKSPACE] to Exit", startX, startY + 160, 15, GRAY);
}

// Menu CSIE (Sudah disesuaikan dengan input main.c)
void DrawCSIEMenu(int screenWidth, int screenHeight) {
    DrawPopUpBase(screenWidth, screenHeight, "CSIE DEPARTMENT");
    int startX = screenWidth/2 - 210;
    int startY = screenHeight/2 - 60;

    DrawText("1. Attend Calculus Class", startX, startY, 20, DARKBLUE);
    
    DrawText("Press [BACKSPACE] to Exit", startX, startY + 160, 15, GRAY);
}



#endif