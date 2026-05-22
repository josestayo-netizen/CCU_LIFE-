    //gcc main.c -o ../CCU_LIFE.exe -I ../Include libraylib.a -lopengl32 -lgdi32 -lwinmm
    #include "raylib.h"
    #include "player.h"
    #include "Building.h"
    #include "ui.h"

    typedef enum { STATE_EXPLORING, STATE_MENU, STATE_DIALOG } GameState; // Tambah STATE_DIALOG
    GameState currentState = STATE_DIALOG;

    int currentDay = 1;
    int storyIndex = 0;
    const char* currentDialogue = "";

    void UpdateStory() {
        if (currentDay == 1) {
            if (storyIndex == 0) currentDialogue = "Hi Andreas, Welcome to Chung Cheng University!";
            else if (storyIndex == 1) currentDialogue = "I hope you're doing well!";
            else if (storyIndex == 2) currentDialogue = "BELI PARFUM MYKONOS SEKARANG JUGA.";
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

        Player andreas; 
        InitPlayer(&andreas);

        Building buildings[] = {
            {{ 100, 100, 200, 150 }, BROWN, "Dormitory", true},
            {{ 1400, 200, 300, 200 }, GRAY, "CSIE Building", true},
            {{ 800, 800, 250, 150 }, ORANGE, "Cafeteria", true}
        };
        int buildingCount = 3;

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
        camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };

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

        // --- 2. DRAWING ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
                DrawBuildings(buildings, buildingCount); 
                DrawCircleV(andreas.position, 20, BLUE);
            EndMode2D();

            if (currentState == STATE_EXPLORING) {
            DrawMissionHUD(screenWidth, andreas);}

            // Layer Dialog (Paling depan)
            if (currentState == STATE_DIALOG) {
                DrawDialogueScene(screenWidth, screenHeight, "Senior Student", currentDialogue);
            }

            // Layer Menu
            if (currentState == STATE_MENU) {
                if (buildingActive == 0) DrawPopUpBase(screenWidth, screenHeight, "DORMITORY - Press [1] to Sleep");
                else if (buildingActive == 1) DrawCSIEMenu(screenWidth, screenHeight);
                else if (buildingActive == 2) DrawCafeteriaMenu(screenWidth, screenHeight);
            }

            // HUD UI
            if (andreas.isBlurry) DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GRAY, 0.4f));
            DrawText(TextFormat("Day: %d | Energy: %.1f | Stress: %.1f", currentDay, andreas.energy, andreas.stress), 10, 10, 25, BLACK);

            if (buildingActive != -1 && currentState == STATE_EXPLORING) {
                DrawText("Press [E] to Interact", screenWidth/2 - 100, screenHeight - 100, 20, DARKGRAY);
            }

        EndDrawing();}
    
        CloseWindow();
        return 0; } 
