    //gcc main.c -o ../CCU_LIFE.exe -I ../Include libraylib.a -lopengl32 -lgdi32 -lwinmm
    #include "raylib.h"
    #include "player.h"
    #include "Building.h"
    #include "ui.h"
    #include <math.h>

    typedef enum { STATE_MAIN_MENU, STATE_EXPLORING, STATE_MENU, STATE_DIALOG } GameState; // Tambah STATE_DIALOG
    GameState currentState = STATE_MAIN_MENU;

    int currentDay = 1;
    int storyIndex = 0;
    const char* currentDialogue = "";

    void UpdateStory() {
        if (currentDay == 1) {
            if (storyIndex == 0) currentDialogue = "Hi Andreas, Welcome to Chung Cheng University!";
            else if (storyIndex == 1) currentDialogue = "I hope you're doing well!, here's you schedule today!";
            else if (storyIndex == 2) currentDialogue = "Have fun!";
            else { currentState = STATE_EXPLORING; storyIndex = 0; }
        } 
        else if (currentDay == 2) {
            if (storyIndex == 0) currentDialogue = "Goodmorning, Andreas, How are you?.";
            else if (storyIndex == 1) currentDialogue = "Badanmu lemas? Pergi ke Cafeteria dan makan Dada Ayam.";
            else if (storyIndex == 2) currentDialogue = "Protein itu penting buat fokus koding dan push-up!";
            else { currentState = STATE_EXPLORING; storyIndex = 0; }
        }
    }

    int main() {
        int screenWidth = 1280; 
        int screenHeight = 720;

        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT); 
        InitWindow(screenWidth, screenHeight, "CCU LIFE - Andreas Adventure");
        const int targetWidth = 1280;
        const int targetHeight = 720;

        RenderTexture2D target = LoadRenderTexture(targetWidth, targetHeight);
        SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

        Player andreas; 
        InitPlayer(&andreas);

        Building buildings[] = {
            {{ 100, 100, 200, 150 }, BROWN, "Dormitory", true},
            {{ 1400, 200, 300, 200 }, GRAY, "College Of Science", true},
            {{ 800, 800, 250, 150 }, ORANGE, "Cafeteria", true},
            {{ 100, 800, 250, 150 }, BLUE, "General Building", true},
            {{ 1400, 800, 250, 150 }, GREEN, "Language Center", true},
            
        };
        int buildingCount = 5;

        SetTargetFPS(60);

        Camera2D camera = { 0 };
        camera.target = andreas.position;
        camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        while (!WindowShouldClose()) {
        // --- 1. UPDATE LOGIC ---
        Vector2 oldPosition = andreas.position;
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        if (currentState == STATE_MAIN_MENU) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        currentState = STATE_DIALOG;
        storyIndex = 0; // Pindah ke dialog pembuka game
        }
        }

        // A. Logika Dialog (Hanya Update Story & Spasi)
        if (currentState == STATE_DIALOG) {
            UpdateStory(); 
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                storyIndex++;
            }
        }

        // B. Logika Movement (Hanya saat jalan-jalan)
        if (currentState == STATE_EXPLORING) {
            if (IsKeyDown(KEY_W)) andreas.position.y -= andreas.speed;
            if (IsKeyDown(KEY_S)) andreas.position.y += andreas.speed;
            if (IsKeyDown(KEY_A)) andreas.position.x -= andreas.speed;
            if (IsKeyDown(KEY_D)) andreas.position.x += andreas.speed;

            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
                andreas.energy -= 0.01f; 
            }
        }

        // C. Collision & Interaction Detection
        for (int i = 0; i < buildingCount; i++) {
            if (CheckCollisionCircleRec(andreas.position, 20, buildings[i].area)) {
                andreas.position = oldPosition;
                break; 
            }
        }

        int buildingActive = GetNearbyBuilding(andreas.position, buildings, buildingCount);
        camera.target = andreas.position;
        camera.offset = (Vector2){ targetWidth/2.0f, targetHeight/2.0f };

        // D. Masuk ke Menu saat tekan E
        if (buildingActive != -1 && IsKeyPressed(KEY_E) && currentState == STATE_EXPLORING) {
            currentState = STATE_MENU;
        }

        // E. LOGIKA DALAM MENU (Dorm, CSIE, Cafeteria)
        if (currentState == STATE_MENU) {
            // Tombol keluar menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = STATE_EXPLORING;
            }

            // AKSI DI DORM (Ganti Hari)
            if (buildingActive == 0 && IsKeyPressed(KEY_ONE)) {
                currentDay++;
                andreas.energy = 100.0f; 
                andreas.stress -= 20.0f; 
                storyIndex = 0;          
                currentState = STATE_DIALOG; // Pindah ke Dialog Pagi
                TraceLog(LOG_INFO, "Pindah ke hari %d", currentDay);
            }

            // AKSI DI CAFETERIA (Makan)
            if (buildingActive == 2 && IsKeyPressed(KEY_ONE)) {
                andreas.energy += 30.0f;
                if (andreas.energy > 100) andreas.energy = 100;
                currentState = STATE_EXPLORING;
            }

            // AKSI DI CSIE (Kuliah)
            if (buildingActive == 1 && IsKeyPressed(KEY_ONE)) {
                andreas.stress += 10.0f;
                andreas.missions.classAttended = true;
                currentState = STATE_EXPLORING;

            }

            if (buildingActive == 1 && IsKeyPressed(KEY_TWO)) {
                andreas.missions.assignmentsDone = true; // <--- INI UNTUK TUGAS
                currentState = STATE_EXPLORING;
                }
        }

        andreas.isBlurry = (andreas.stress > 80.0f || andreas.energy < 20.0f);
        float scale = fminf((float)screenWidth / targetWidth, (float)screenHeight / targetHeight);

    // Langkah A: Gambar semuanya ke Virtual Canvas terlebih dahulu
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        // Dunia Game (Menggunakan Kamera)
        BeginMode2D(camera);
            DrawBuildings(buildings, buildingCount); 
            DrawCircleV(andreas.position, 20, BLUE);
        EndMode2D();

        if (currentState == STATE_MAIN_MENU) {
        ClearBackground(RAYWHITE); // Bersihkan layar dengan warna putih/abu terang

         // Judul Game
        DrawText("CCU LIFE", targetWidth / 2 - 150, targetHeight / 2 - 100, 60, DARKBLUE);
        DrawText("A Student Adventure", targetWidth / 2 - 160, targetHeight / 2 - 30, 25, DARKGRAY);

        // Petunjuk Tekan Tombol (Efek berkedip sederhana bisa dibuat nanti, ini yang statis dulu)
        DrawText("Press [ENTER] or [SPACE] to Start", targetWidth / 2 - 200, targetHeight / 2 + 100, 20, BLACK);
    
        // Info Tambahan di bawah (Opsional)
        DrawText("Created by Group 3", 20, targetHeight - 40, 20, GRAY);
        }

        // UI & HUD (Semua ukuran layar menggunakan targetWidth & targetHeight agar konstan)
        if (currentState == STATE_EXPLORING) {
            DrawMissionHUD(targetWidth, andreas);
        }

        if (currentState == STATE_DIALOG) {
            DrawDialogueScene(targetWidth, targetHeight, "Senior Student", currentDialogue);
        }

        if (currentState == STATE_MENU) {
            if (buildingActive == 0) DrawPopUpBase(targetWidth, targetHeight, "DORMITORY - Press [1] to Sleep");
            else if (buildingActive == 1) DrawCSIEMenu(targetWidth, targetHeight);
            else if (buildingActive == 2) DrawCafeteriaMenu(targetWidth, targetHeight);
        }

        if (andreas.isBlurry) DrawRectangle(0, 0, targetWidth, targetHeight, Fade(GRAY, 0.4f));
        
        // Ukuran font biarkan tetap 25 & 20 statis, karena canvas virtual ini skalanya otomatis berubah
        DrawText(TextFormat("Day: %d | Energy: %.1f | Stress: %.1f", currentDay, andreas.energy, andreas.stress), 10, 10, 25, BLACK);

        if (buildingActive != -1 && currentState == STATE_EXPLORING) {
            DrawText("Press [E] to Interact", targetWidth / 2 - 100, targetHeight - 100, 20, DARKGRAY);
        }
        EndTextureMode();

        // --- 2. DRAWING ---
        BeginDrawing();
            ClearBackground(BLACK);

            
            DrawTexturePro(
            target.texture,
            (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
            (Rectangle){ (screenWidth - ((float)targetWidth * scale)) * 0.5f, 
                         (screenHeight - ((float)targetHeight * scale)) * 0.5f, 
                         (float)targetWidth * scale, 
                         (float)targetHeight * scale },
            (Vector2){ 0, 0 }, 
            0.0f, 
            WHITE
        );
        EndDrawing();}
        UnloadRenderTexture(target);
        CloseWindow();
        return 0; } 
