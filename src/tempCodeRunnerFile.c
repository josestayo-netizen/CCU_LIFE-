// src/main.c
#include "raylib.h"
#include "player.h"

int main() {
   // 1. Deklarasi resolusi dasar (sebagai starting point)
    int screenWidth = 1280; 
    int screenHeight = 720;

    // Tambahkan baris ini SEBELUM InitWindow
    // FLAG_WINDOW_RESIZABLE: Agar window bisa ditarik/maximize
    // FLAG_MSAA_4X_HINT: Agar pinggiran objek (lingkaran Andreas) lebih halus
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT); 

    InitWindow(screenWidth, screenHeight, "CCU LIFE - Andreas Adventure");

    // 2. Inisialisasi Player
    Player andreas; 
    InitPlayer(&andreas);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        // --- Logic: Movement WASD (8-way movement) --- [cite: 63, 131]
        if (IsKeyDown(KEY_W)) andreas.position.y -= andreas.speed;
        if (IsKeyDown(KEY_S)) andreas.position.y += andreas.speed;
        if (IsKeyDown(KEY_A)) andreas.position.x -= andreas.speed;
        if (IsKeyDown(KEY_D)) andreas.position.x += andreas.speed;

        // --- Logic: Energy Depletion (Stamina fisik berkurang saat bergerak) --- [cite: 24, 98, 132]
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
            andreas.energy -= 0.01f; 
        }

        // --- Logic: Visual Feedback (Triggered by Stress or Low Energy) --- [cite: 40, 112]
        // Layar akan berubah jika Stress > 80 atau Energy < 20
        if (andreas.stress > 80.0f || andreas.energy < 20.0f) {
            andreas.isBlurry = true;
        } else {
            andreas.isBlurry = false;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Sekarang screenWidth dan screenHeight sudah dikenali!
            if (andreas.isBlurry) {
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GRAY, 0.3f));
            }

            // Gambar Karakter Andreas (Representasi visual top-down) [cite: 4, 82]
            DrawCircleV(andreas.position, 20, BLUE);

            // Tampilkan HUD Status untuk Andreas [cite: 22, 96]
            DrawText(TextFormat("Name: Andreas"), 10, 10, 20, BLACK);
            DrawText(TextFormat("Energy: %.1f", andreas.energy), 10, 40, 20, DARKGREEN); // [cite: 24, 98]
            DrawText(TextFormat("Stress: %.1f", andreas.stress), 10, 70, 20, RED);       // [cite: 25, 99]
            
            DrawText("Andreas is exploring CCU campus...", 250, 1030, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}