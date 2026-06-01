//gcc main.c -o ../CCU_LIFE.exe -I ../Include libraylib.a -lopengl32 -lgdi32 -lwinmm
#include "raylib.h"
#include "player.h"
#include "Building.h"
#include "ui.h"
#include "mini_games.h"
#include <math.h>
#include <stdlib.h> // Untuk rand() dan atoi()

MiniScreen currentMiniScreen = MINI_NONE;
MathGame mathGame;
MandarinGame mandarinGame;
EnglishGame englishGame;
MissionGame missionGame;
char lastGameTitle[64] = "";
int lastScore = 0;
int lastTotal = 0;
int triggerMissionType = 0;
bool isSleepingEnding = false; 
float endingFadeAlpha = 0.0f;

typedef enum { STATE_MAIN_MENU, STATE_EXPLORING, STATE_MENU, STATE_DIALOG, STATE_MINIGAME } GameState;
GameState currentState = STATE_MAIN_MENU;

int currentDay = 1;
int storyIndex = 0;
const char* currentDialogue = "";

void UpdateStory() {
    if (currentDay == 1) {
        if (storyIndex == 0) currentDialogue = "Hi Andreas, Welcome to Chung Cheng University!";
        else if (storyIndex == 1) currentDialogue = "I hope you're doing well! I'll give your task in the right up corner!";
        else if (storyIndex == 2) currentDialogue = "Be careful with your stress and energy level, \nmake sure you eat in the cafetaria and \ngo to the dorm to move to the next day!";
        else { currentState = STATE_EXPLORING; storyIndex = 0; }
    } 
    else if (currentDay == 2) {
        if (storyIndex == 0) currentDialogue = "Good morning, Andreas. How are you?";
        else if (storyIndex == 1) currentDialogue = "Hope you're doing well.... ";
        else if (storyIndex == 2) currentDialogue = "Don't forget to do your assigments today!";
        else { currentState = STATE_EXPLORING; storyIndex = 0; }
    }
    else if (currentDay == 3) {
        if (storyIndex == 0) currentDialogue = "Hi Andreas!";
        else if (storyIndex == 1) currentDialogue = "This is your last day!";
        else if (storyIndex == 2) currentDialogue = "GOOD LUCK ON YOUR FINAL EXAM !";
        else { currentState = STATE_EXPLORING; storyIndex = 0; }
    }
} // <-- (Selesai Perbaikan Masalah 1: Kurung penutup fungsi UpdateStory sekarang sudah ada)

int main() {
    int screenWidth = 1280; 
    int screenHeight = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT); 
    InitWindow(screenWidth, screenHeight, "CCU LIFE - Andreas Adventure");
    const int targetWidth = 1280;
    const int targetHeight = 720;
    Texture2D backgroundMap = LoadTexture("map.png");
    Texture2D andreasTexture = LoadTexture("idle.png");

    RenderTexture2D target = LoadRenderTexture(targetWidth, targetHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    Player andreas; 
    InitPlayer(&andreas);
    andreas.position = (Vector2){ 824.0f, 1712.0f };

    Building buildings[] = {
        // Format: {{ X, Y, Lebar, Tinggi }, WARNA, "Nama_Gedung", true}
        
        // 1. Dormitory (Asrama - tempat start point Andreas)
        {{ 750, 1500, 150, 150 }, BROWN, "Dormitory", true},
        
        // 2. College Of Science
        {{ 1444, 1252, 150, 150 }, GRAY, "College Of Science", true},
        
        // 3. Cafeteria
        {{ 1992, 1514, 150, 150 }, ORANGE, "Cafeteria", true},
        
        // 4. General Building
        {{ 2042, 970, 150, 150 }, BLUE, "General Building", true},
        
        // 5. Language Center
        {{ 1008, 976, 150, 150 }, GREEN, "Language Center", true},
    };
    int buildingCount = 5;

    SetTargetFPS(60);
    InitAudioDevice();
    Music gameMusic = LoadMusicStream("bgm.mp3");
    SetMusicVolume(gameMusic, 0.5f);
    Music minigameMusic = LoadMusicStream("minigame.mp3"); // Siapkan file .mp3 baru
    SetMusicVolume(minigameMusic, 0.5f);
    Sound fxFootstep = LoadSound("foot.mp3");
    SetSoundVolume(fxFootstep, 0.5f);
    PlayMusicStream(gameMusic);

    Camera2D camera = { 0 };
    camera.target = andreas.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.5f;

    Texture2D seniorPortrait = LoadTexture("Seniorstudent.png");
    
    float frameWidth =64.0f;  // Lebar 1 kotak karakter
    float frameHeight = 64.0f; // Tinggi 1 kotak karakter

    int directionRow = 2;      // Default: Baris 2 (Hadap Depan) saat game mulai
    int currentFrame = 0;      // Kolom frame (0 atau 1)
    int frameCounter = 0;



    while (!WindowShouldClose()) {
        // --- 1. UPDATE LOGIC ---
        Vector2 oldPosition = andreas.position;
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        float dt = GetFrameTime();
        UpdateMusicStream(gameMusic);
        UpdateMusicStream(minigameMusic);

        // Perbaikan Masalah 2: Logika isSleepingEnding dibungkus dengan benar
        if (isSleepingEnding) {
            andreas.speed = 0; 
            if (endingFadeAlpha < 1.0f) {
                endingFadeAlpha += dt * 0.4f; 
            }
            if (endingFadeAlpha >= 1.0f && currentState != STATE_DIALOG && storyIndex < 100) {
                currentState = STATE_DIALOG;
                storyIndex = 100; 
                UpdateStory();
            }
        }
    
        if (currentState == STATE_MAIN_MENU) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                currentState = STATE_DIALOG;
                storyIndex = 0;
            }
        }
        else if (currentState == STATE_DIALOG) { // Menggunakan else if agar struktur logika lurus
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                storyIndex++;
            }
            UpdateStory(); 
        }
        else if (currentState == STATE_EXPLORING) {
        bool isMoving = false;

        if (IsKeyDown(KEY_W)) { andreas.position.y -= andreas.speed; directionRow = 0; isMoving = true; } // Baris 0
        if (IsKeyDown(KEY_A)) { andreas.position.x -= andreas.speed; directionRow = 1; isMoving = true; } // Baris 1
        if (IsKeyDown(KEY_S)) { andreas.position.y += andreas.speed; directionRow = 2; isMoving = true; } // Baris 2
        if (IsKeyDown(KEY_D)) { andreas.position.x += andreas.speed; directionRow = 3; isMoving = true; } // Baris 3

        if (isMoving) {
        andreas.energy -= 0.01f; 
        if (!IsSoundPlaying(fxFootstep)) {
            PlaySound(fxFootstep);
        }
        } else {
        // Jika Andreas berhenti jalan, langsung matikan suara langkahnya instan
        StopSound(fxFootstep);

        }
        
        // --- LOGIKA ANIMASI ALTERNATIF FRAME (KIRIMAN KAMU ADA 2 KOLOM) ---
        frameCounter++;
        if (frameCounter >= 30) { // Setiap 30 frame (sekitar 0.5 detik), ganti pose kaki/napas
        frameCounter = 0;
        currentFrame = (currentFrame + 1) % 2; // Bolak-balik antara Kolom 0 dan Kolom 1
        }
            if (andreas.stress < 0.0f) andreas.stress = 0.0f;
        }

        // Collision & Interaction Detection
        for (int i = 0; i < buildingCount; i++) {
            if (CheckCollisionCircleRec(andreas.position, 20, buildings[i].area)) {
                andreas.position = oldPosition;
                break; 
            }
        }

        int buildingActive = GetNearbyBuilding(andreas.position, buildings, buildingCount);
        camera.target = andreas.position;
        camera.offset = (Vector2){ targetWidth/2.0f, targetHeight/2.0f };

        if (buildingActive != -1 && IsKeyPressed(KEY_E) && currentState == STATE_EXPLORING) {
            currentState = STATE_MENU;
        }

        // LOGIKA DALAM MENU
        if (currentState == STATE_MENU) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                currentState = STATE_EXPLORING;
            }

            if (buildingActive == 0 && IsKeyPressed(KEY_ONE)) {
                currentDay++;
                andreas.energy += 10.0f; 
                andreas.stress -= 5.0f; 
                if (andreas.stress < 0.0f) andreas.stress = 0.0f;
                andreas.missions.classAttended = false;
                andreas.missions.assignmentsDone = false;
                andreas.missions.libraryVisited = false;
                storyIndex = 0;          
                currentState = STATE_DIALOG;
                TraceLog(LOG_INFO, "Move to day %d", currentDay);
            }     

            if (buildingActive == 2 && IsKeyPressed(KEY_ONE)) {
                andreas.energy += 10.0f;
                andreas.stress -= 10.0f; 
                andreas.score -= 1.0f;
                if (andreas.energy > 100) andreas.energy = 100;
                currentState = STATE_EXPLORING;
            }

            if (buildingActive == 1 && IsKeyPressed(KEY_ONE)) {
                triggerMissionType = 1;      
                StartMathGame();             
                currentState = STATE_MINIGAME; 
            }

            if (buildingActive == 3 && IsKeyPressed(KEY_ONE)) {
                triggerMissionType = 4; 
                StartMissionGame();
                currentState = STATE_MINIGAME;
            }
                
            if (buildingActive == 4) {
                if (currentDay == 1) {
                    if (IsKeyPressed(KEY_TWO)) {
                        triggerMissionType = 2; 
                        StartEnglishGame();     
                        currentState = STATE_MINIGAME; 
                    }
                } 
                else {
                    if (IsKeyPressed(KEY_ONE)) {
                        triggerMissionType = 3; 
                        StartMandarinGame();    
                        currentState = STATE_MINIGAME; 
                    }
                }
            }
        }   

        // LOGIKA KONTROL INPUT MINI-GAME
        if (currentState == STATE_MINIGAME) {
            float timeMultiplier = 1.0f;
            if (andreas.stress > 40.0f) {
            timeMultiplier = 2.0f; 
            } else if (andreas.stress > 30.0f) {
            timeMultiplier = 1.5f; }
            float modifiedDt = dt * timeMultiplier;
            
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
    
                        int nextWord = englishGame.current;
                        while (nextWord == englishGame.current) {
                            nextWord = rand() % EnglishCount();
                        }
                        englishGame.current = nextWord; 
    
                        englishGame.input[0] = '\0'; 
                        englishGame.inputLen = 0;
                    }
                }
            } 
            else if (currentMiniScreen == MINI_MISSION) {
                MissionGame *g = &missionGame;
                
                if (g->phase != MISSION_DONE) {
                    UpdateTimer(&g->base, "General Subject Quiz", dt);
                }

                if (g->phase == MISSION_PLAYING && g->base.timeLeft > 0.0f) {
                    if (IsKeyPressed(KEY_DOWN)) g->selectedOption = (g->selectedOption + 1) % 4;
                    if (IsKeyPressed(KEY_UP))   g->selectedOption = (g->selectedOption + 3) % 4;

                    if (IsKeyPressed(KEY_ENTER)) {
                        g->base.total++;
                        if (g->selectedOption == missionQuestions[g->currentQuestion].correctAnswer) {
                            g->base.score++; 
                            g->isCorrect = true;
                        } else {
                            g->isCorrect = false;
                        }
                        g->phase         = MISSION_FEEDBACK;
                        g->feedbackTimer = 1.0f; 
                    }
                } 
                else if (g->phase == MISSION_FEEDBACK) {
                    g->feedbackTimer -= dt;
                    if (g->feedbackTimer <= 0.0f) {
                        g->currentQuestion++;
                        g->selectedOption = 0;
                        if (g->currentQuestion >= MAX_MISSION_QUESTIONS) {
                            g->phase = MISSION_DONE;
                            FinishTask("General Subject Quiz", g->base);
                        } else {
                            g->phase = MISSION_PLAYING;
                        }
                    }
                }
            }
            else if (currentMiniScreen == MINI_RESULT) {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE)) { 

                    int pointsEarned = 0;
        
                    if (triggerMissionType == 1) {
                    // Contoh: Setiap soal matematika yang benar dikali 2 poin
                    pointsEarned = mathGame.base.score * 2; 
                    } 
                    else if (triggerMissionType == 2) {
                    pointsEarned = englishGame.base.score * 2;
                    } 
                    else if (triggerMissionType == 3) {
                    pointsEarned = mandarinGame.base.score * 2;
                    } 
                    else if (triggerMissionType == 4) {
                    pointsEarned = missionGame.base.score * 2;
                    }
                    if (triggerMissionType == 1) {
                     // Skor hanya bertambah kalau Andreas belum pernah menghadiri kelas hari ini
                    if (!andreas.missions.classAttended) {
                    andreas.score += pointsEarned;
                    }
                    andreas.missions.classAttended = true; 
                    } 
                    else if (triggerMissionType == 2 || triggerMissionType == 3 || triggerMissionType == 4) {
                    // Skor hanya bertambah kalau tugas hari ini belum selesai
                    if (!andreas.missions.assignmentsDone) {
                    andreas.score += pointsEarned;
                    }
                    andreas.missions.assignmentsDone = true; 
                    }
                    andreas.stress += 30.0f;

                    if (currentDay == 3 && andreas.missions.assignmentsDone) {
                        currentMiniScreen = MINI_NONE;
                        currentState = STATE_EXPLORING; 
                        isSleepingEnding = true; 
                    } 
                    else {
                        currentState = STATE_EXPLORING; 
                        currentMiniScreen = MINI_NONE;
                    }
                }
            }
        }

        andreas.isBlurry = (andreas.stress > 50.0f || andreas.energy < 30.0f);
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

            if (IsMusicStreamPlaying(gameMusic)) {
                StopMusicStream(gameMusic);}
            if (!IsMusicStreamPlaying(minigameMusic)) {
                PlayMusicStream(minigameMusic);}

            
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
            else if (currentMiniScreen == MINI_MISSION) {
                DrawHeader("General Subject Quiz", missionGame.base);
                MissionQuestion mq = missionQuestions[missionGame.currentQuestion];
                
                if (missionGame.phase == MISSION_PLAYING) {
                    DrawCenteredText(mq.question, 180, 32, BLACK);
                    for (int i = 0; i < 4; i++) {
                        Color btnColor = (missionGame.selectedOption == i) ? SKYBLUE : LIGHTGRAY;
                        DrawButtonBox(targetWidth / 2 - 300, 280 + (i * 90), 600, 65, 
                                      TextFormat("%d. %s", i + 1, mq.options[i]), btnColor);
                    }
                    DrawCenteredText("Use [UP/DOWN] to select, [ENTER] to submit", 660, 18, DARKGRAY);
                } 
                else if (missionGame.phase == MISSION_FEEDBACK) {
                    if (missionGame.isCorrect) {
                        DrawCenteredText("CORRECT!", targetHeight / 2 - 50, 50, GREEN);
                    } else {
                        DrawCenteredText("WRONG ANSWER!", targetHeight / 2 - 50, 50, RED);
                        int correctIdx = mq.correctAnswer;
                        DrawCenteredText(TextFormat("Correct answer was: %s", mq.options[correctIdx]), 
                                         targetHeight / 2 + 20, 24, DARKGRAY);
                    }
                }
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

            if (IsMusicStreamPlaying(minigameMusic)) {
                StopMusicStream(minigameMusic);   // Matikan musik mini-game
            }
            if (!IsMusicStreamPlaying(gameMusic) && currentState != STATE_MAIN_MENU) {
                PlayMusicStream(gameMusic);       // Nyalakan lagi musik eksplorasi
            }

            BeginMode2D(camera);
                DrawTextureEx(backgroundMap, (Vector2){ 0, 0 }, 0.0f, 2.0f, WHITE);
                DrawBuildings(buildings, buildingCount); 
                //DrawCircleV(andreas.position, 20, BLUE);
                Rectangle sourceRec = { (float)currentFrame * frameWidth, (float)directionRow * frameHeight, frameWidth, frameHeight};
    
                Rectangle destRec = { andreas.position.x, andreas.position.y, frameWidth * 1.0f, frameHeight * 1.0f };
    
                // Set titik poros di tengah bawah kaki karakter
                Vector2 origin = { (frameWidth * 1.0f) / 2, (frameHeight * 1.0f) / 2 };
    
                // Gambar si Andreas!
                DrawTexturePro(andreasTexture, sourceRec, destRec, origin, 0.0f, WHITE);
            EndMode2D();

            if (currentState == STATE_EXPLORING) {
                DrawMissionHUD(targetWidth, andreas, currentDay);
            }

            if (currentState == STATE_DIALOG) {
                DrawDialogueScene(targetWidth, targetHeight, "Senior Student", currentDialogue, seniorPortrait);
            }

            if (currentState == STATE_MENU) {
                if (buildingActive == 0) DrawPopUpBase(targetWidth, targetHeight, "DORMITORY - Press [1] to Sleep");
                else if (buildingActive == 1) DrawCSIEMenu(targetWidth, targetHeight); 
                else if (buildingActive == 2) DrawCafeteriaMenu(targetWidth, targetHeight);
                else if (buildingActive == 3) DrawGeneralMenu(targetWidth, targetHeight);
                else if (buildingActive == 4) DrawLanguageCenterMenu(targetWidth, targetHeight, currentDay);
            }

            if (andreas.isBlurry) DrawRectangle(0, 0, targetWidth, targetHeight, Fade(GRAY, 0.4f));
            
            DrawRectangle(5, 5, 620, 40, Fade(WHITE, 0.8f));
            
            DrawText(TextFormat("Day: %d | Energy: %.1f | Stress: %.1f | Score : %d pts", currentDay, andreas.energy, andreas.stress, andreas.score), 10, 10, 25, BLACK);
            if (buildingActive != -1 && currentState == STATE_EXPLORING) {
                DrawRectangle(targetWidth / 2 - 120, targetHeight - 115, 255, 45, Fade(WHITE, 0.8f));
                DrawText("Press [E] to Interact", targetWidth / 2 - 100, targetHeight - 100, 20, DARKGRAY);
            }
        }
        
        if (isSleepingEnding) {
                DrawEndingScene(targetWidth, targetHeight, endingFadeAlpha, andreas.score);
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
    UnloadTexture(backgroundMap);
    UnloadTexture(seniorPortrait);
    UnloadTexture(andreasTexture);
    UnloadSound(fxFootstep);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}