//gcc main.c -o ../CCU_LIFE.exe -I ../Include libraylib.a -lopengl32 -lgdi32 -lwinmm
#include "raylib.h"
#include "player.h"
#include "Building.h"
#include "ui.h"
#include "mini_games.h"
#include <math.h>

MiniScreen currentMiniScreen = MINI_NONE;
MathGame mathGame;
MandarinGame mandarinGame;
EnglishGame englishGame;
char lastGameTitle[64] = "";
int lastScore = 0;
int lastTotal = 0;
int triggerMissionType = 0;

typedef enum { STATE_MAIN_MENU, STATE_EXPLORING, STATE_MENU, STATE_DIALOG, STATE_MINIGAME } GameState;
GameState currentState = STATE_MAIN_MENU;

int currentDay = 1;
int storyIndex = 0;
const char* currentDialogue = "";

void UpdateStory() {
    if (currentDay == 1) {
        if (storyIndex == 0) currentDialogue = "Hi Andreas, Welcome to Chung Cheng University!";
        else if (storyIndex == 1) currentDialogue = "I hope you're doing well!, here's your schedule today!";
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
        float dt = GetFrameTime();

        if (currentState == STATE_MAIN_MENU) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                currentState = STATE_DIALOG;
                storyIndex = 0;
            }
        }

        // A. Logika Dialog
        if (currentState == STATE_DIALOG) {
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                storyIndex++;
            }
            UpdateStory(); 

        }

        // B. Logika Movement
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

        // E. LOGIKA DALAM MENU (Dorm, CSIE, Cafeteria, Language Center)
        if (currentState == STATE_MENU) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                currentState = STATE_EXPLORING;
            }

            // AKSI DI DORM (Index 0)
            if (buildingActive == 0 && IsKeyPressed(KEY_ONE)) {
                currentDay++;
                andreas.energy = 100.0f; 
                andreas.stress -= 20.0f; 
                andreas.missions.classAttended = false;
                andreas.missions.assignmentsDone = false;
                andreas.missions.libraryVisited = false;
                storyIndex = 0;          
                currentState = STATE_DIALOG;
                TraceLog(LOG_INFO, "Move to day %d", currentDay);
            }     

            // AKSI DI CAFETERIA (Index 2)
            if (buildingActive == 2 && IsKeyPressed(KEY_ONE)) {
                andreas.energy += 30.0f;
                if (andreas.energy > 100) andreas.energy = 100;
                currentState = STATE_EXPLORING;
            }

            // AKSI DI CSIE (Index 1) - Hanya Kuis Matematika (Kuliah)
            if (buildingActive == 1 && IsKeyPressed(KEY_ONE)) {
                triggerMissionType = 1;      // Tanda misi Kalkulus
                StartMathGame();             
                currentState = STATE_MINIGAME; 
            }
            
            // AKSI DI LANGUAGE CENTER (Index 4) - Kuis Mandarin & English
            if (buildingActive == 4) {
                if (currentDay == 1) {
                    // Hanya bisa akses kuis Inggris di Hari 1
                    if (IsKeyPressed(KEY_TWO)) {
                        triggerMissionType = 2; // Tanda misi Inggris
                        StartEnglishGame();     
                        currentState = STATE_MINIGAME; 
                    }
                } 
                else {
                    // Hanya bisa akses kuis Mandarin di Hari 2 atau lebih
                    if (IsKeyPressed(KEY_ONE)) {
                        triggerMissionType = 3; // Tanda misi Mandarin
                        StartMandarinGame();    
                        currentState = STATE_MINIGAME; 
                    }
                }
            }
        }   

        // F. LOGIKA KONTROL INPUT MINI-GAME
        if (currentState == STATE_MINIGAME) {
            if (IsKeyPressed(KEY_BACKSPACE) && currentMiniScreen != MINI_RESULT) {
                currentState = STATE_MENU; 
            }

            if (currentMiniScreen == MINI_MATH) {
                if (!UpdateTimer(&mathGame.base, "Mathematics Quiz", dt)) {
                    int ch = GetCharPressed();
                    while (ch > 0) { AddInputChar(mathGame.input, &mathGame.inputLen, ch, true); ch = GetCharPressed(); }
                    if (IsKeyPressed(KEY_BACKSPACE)) BackspaceInput(mathGame.input, &mathGame.inputLen);
                    if (IsKeyPressed(KEY_ENTER) && mathGame.inputLen > 0) {
                        if (atoi(mathGame.input) == mathGame.correct) mathGame.base.score++;
                        mathGame.base.total++;
                        GenerateMathQuestion();
                    }
                }
            } 
            else if (currentMiniScreen == MINI_MANDARIN) {
                if (!UpdateTimer(&mandarinGame.base, "Mandarin Pinyin Quiz", dt)) {
                    int chosen = -1;
                    if (IsKeyPressed(KEY_A)) chosen = 0; if (IsKeyPressed(KEY_B)) chosen = 1;
                    if (IsKeyPressed(KEY_C)) chosen = 2; if (IsKeyPressed(KEY_D)) chosen = 3;
                    if (chosen != -1) {
                        if (chosen == mandarinQuestions[mandarinGame.current].correctIndex) mandarinGame.base.score++;
                        mandarinGame.base.total++;
                        mandarinGame.current = rand() % MandarinCount();
                    }
                }
            } 
            else if (currentMiniScreen == MINI_ENGLISH) {
                if (!UpdateTimer(&englishGame.base, "English Word Scramble", dt)) {
                    int ch = GetCharPressed();
                    while (ch > 0) { AddInputChar(englishGame.input, &englishGame.inputLen, ch, false); ch = GetCharPressed(); }
                    if (IsKeyPressed(KEY_BACKSPACE)) BackspaceInput(englishGame.input, &englishGame.inputLen);
                    if (IsKeyPressed(KEY_ENTER) && englishGame.inputLen > 0) {
                       if (EqualsIgnoreCase(englishGame.input, englishWords[englishGame.current].answer)) englishGame.base.score++;
                       englishGame.base.total++;
    
                        // 💡 TRICK BARU: Acak terus sampai dapat indeks yang beda dengan soal sekarang
                        int nextWord = englishGame.current;
                        while (nextWord == englishGame.current) {
                        nextWord = rand() % EnglishCount();}
                        englishGame.current = nextWord; // Set ke kata yang baru
    
                        englishGame.input[0] = '\0'; 
                        englishGame.inputLen = 0;
                    }
                }
            } 
            else if (currentMiniScreen == MINI_RESULT) {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE)) { 
                    if (triggerMissionType == 1) {
                        andreas.stress += 10.0f; 
                        andreas.missions.classAttended = true; // Selesai Kalkulus
                    } 
                    else if (triggerMissionType == 2 || triggerMissionType == 3) {
                        andreas.missions.assignmentsDone = true; 
                    }
                    currentState = STATE_EXPLORING; 
                    currentMiniScreen = MINI_NONE;
                }
            }
        }

        andreas.isBlurry = (andreas.stress > 80.0f || andreas.energy < 20.0f);
        float scale = fminf((float)screenWidth / targetWidth, (float)screenHeight / targetHeight);

        // --- 2. DRAWING LOGIC ---
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        if (currentState == STATE_MAIN_MENU) {
            DrawText("CCU LIFE", targetWidth / 2 - 150, targetHeight / 2 - 100, 60, DARKBLUE);
            DrawText("A Student Adventure", targetWidth / 2 - 160, targetHeight / 2 - 30, 25, DARKGRAY);
            DrawText("Press [ENTER] or [SPACE] to Start", targetWidth / 2 - 200, targetHeight / 2 + 100, 20, BLACK);
            DrawText("Created by Group 3", 20, targetHeight - 40, 20, GRAY);
        } 
        else if (currentState == STATE_MINIGAME) {
            if (currentMiniScreen == MINI_MATH) {
                DrawHeader("Mathematics Quiz", mathGame.base);
                DrawCenteredText(mathGame.question, 250, 40, BLACK);
                DrawCenteredText(TextFormat("Your Answer: %s", mathGame.input), 350, 30, DARKBLUE);
                DrawCenteredText("Press [ENTER] to Submit", 500, 20, GRAY);
            } 
            else if (currentMiniScreen == MINI_MANDARIN) {
                DrawHeader("Mandarin Pinyin Quiz", mandarinGame.base);
                MandarinQuestion q = mandarinQuestions[mandarinGame.current];
                DrawCenteredText(TextFormat("What is the meaning of: \"%s\"?", q.pinyin), 200, 35, BLACK);
                for (int i = 0; i < 4; i++) {
                    DrawButtonBox(240 + (i % 2) * 420, 320 + (i / 2) * 120, 380, 70, TextFormat("%c. %s", 'A' + i, q.choices[i]), LIGHTGRAY);
                }
            } 
            else if (currentMiniScreen == MINI_ENGLISH) {
                DrawHeader("English Word Scramble", englishGame.base);
                EnglishWord w = englishWords[englishGame.current];
                DrawCenteredText(TextFormat("Unscramble this word: %s", w.scramble), 220, 40, BLACK);
                DrawCenteredText(TextFormat("Hint: %s", w.hint), 300, 24, DARKGRAY);
                DrawCenteredText(TextFormat("Answer: %s", englishGame.input), 400, 32, MAROON);
                DrawCenteredText("Press [ENTER] to Submit", 550, 20, GRAY);
            } 
            else if (currentMiniScreen == MINI_RESULT) {
                DrawRectangle(0, 0, targetWidth, targetHeight, Fade(BLACK, 0.7f));
                DrawCenteredText("TASK COMPLETED!", 200, 50, YELLOW);
                DrawCenteredText(TextFormat("Game: %s", lastGameTitle), 300, 30, WHITE);
                DrawCenteredText(TextFormat("Final Score: %d / %d", lastScore, lastTotal), 380, 35, GREEN);
                DrawCenteredText("Press [ENTER] or [ESC] to Return to Campus", 520, 22, RAYWHITE);
            }
        } 
        else {
            // Tampilan Map Dunia Kampus saat EXPLORING, MENU, DIALOG
            BeginMode2D(camera);
                DrawBuildings(buildings, buildingCount); 
                DrawCircleV(andreas.position, 20, BLUE);
            EndMode2D();

            if (currentState == STATE_EXPLORING) {
                // 🛑 DIPERBAIKI: Mengikuti deklarasi ui.h (2 argumen)
                DrawMissionHUD(targetWidth, andreas, currentDay);
            }

            if (currentState == STATE_DIALOG) {
                DrawDialogueScene(targetWidth, targetHeight, "Senior Student", currentDialogue);
            }

            if (currentState == STATE_MENU) {
                // 🛑 DIPERBAIKI: Menghapus argumen currentDay yang berlebih pada CSIE dan Cafeteria
                if (buildingActive == 0) DrawPopUpBase(targetWidth, targetHeight, "DORMITORY - Press [1] to Sleep");
                else if (buildingActive == 1) DrawCSIEMenu(targetWidth, targetHeight); 
                else if (buildingActive == 2) DrawCafeteriaMenu(targetWidth, targetHeight);
                else if (buildingActive == 4) DrawLanguageCenterMenu(targetWidth, targetHeight, currentDay);
            }

            if (andreas.isBlurry) DrawRectangle(0, 0, targetWidth, targetHeight, Fade(GRAY, 0.4f));
            
            DrawText(TextFormat("Day: %d | Energy: %.1f | Stress: %.1f", currentDay, andreas.energy, andreas.stress), 10, 10, 25, BLACK);

            if (buildingActive != -1 && currentState == STATE_EXPLORING) {
                DrawText("Press [E] to Interact", targetWidth / 2 - 100, targetHeight - 100, 20, DARKGRAY);
            }
        }
        
        EndTextureMode();

        // --- 3. CANVAS RESIZING / SCREEN OUTPUT ---
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
        EndDrawing();
    } 

    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}