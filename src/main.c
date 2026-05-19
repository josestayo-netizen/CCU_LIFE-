#include "raylib.h"
#include "player.h"

typedef struct {
    Rectangle area;
    Color color;
    const char *name;
    bool isInteractable;
} Building;

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

        // Movement
        if (IsKeyDown(KEY_W)) andreas.position.y -= andreas.speed;
        if (IsKeyDown(KEY_S)) andreas.position.y += andreas.speed;
        if (IsKeyDown(KEY_A)) andreas.position.x -= andreas.speed;
        if (IsKeyDown(KEY_D)) andreas.position.x += andreas.speed;

        // Energy & Physics Collision
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
            andreas.energy -= 0.01f; 
        }

        for (int i = 0; i < buildingCount; i++) {
            if (CheckCollisionCircleRec(andreas.position, 20, buildings[i].area)) {
                andreas.position = oldPosition;
                break; 
            }
        }

        // Camera & Interaction Trigger
        camera.target = andreas.position;
        camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };

        int buildingActive = -1; 
        for (int i = 0; i < buildingCount; i++) {  
            Rectangle detectionArea = { 
                buildings[i].area.x - 20, buildings[i].area.y - 20, 
                buildings[i].area.width + 40, buildings[i].area.height + 40 
            };

            if (CheckCollisionCircleRec(andreas.position, 20, detectionArea)) {
                buildingActive = i;
                if (IsKeyPressed(KEY_E)) {
                    if (i == 1) TraceLog(LOG_INFO, "Entering CSIE...");
                    else if (i == 2) {
                        andreas.energy += 20.0f;
                        if (andreas.energy > 100) andreas.energy = 100;
                    }
                }
            }
        }

        // Visual Feedback Logic
        andreas.isBlurry = (andreas.stress > 80.0f || andreas.energy < 20.0f);

        // --- 2. DRAWING ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
                for (int i = 0; i < buildingCount; i++) {
                    DrawRectangleRec(buildings[i].area, buildings[i].color);
                    DrawText(buildings[i].name, buildings[i].area.x + 10, buildings[i].area.y + 10, 20, BLACK);
                }
                DrawCircleV(andreas.position, 20, BLUE); 
            EndMode2D();

            // UI Layer (Statis)
            if (andreas.isBlurry) DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GRAY, 0.4f));

            int dynamicFontSize = screenWidth / 50;
            if (dynamicFontSize < 20) dynamicFontSize = 20;

            if (buildingActive != -1) {
                const char* msg = "Press [E] to Interact";
                int msgWidth = MeasureText(msg, dynamicFontSize);
                DrawText(msg, (screenWidth/2) - (msgWidth/2), screenHeight - 150, dynamicFontSize, DARKGRAY);
            }

            DrawText(TextFormat("Name: Andreas"), 10, 10, dynamicFontSize, BLACK);
            DrawText(TextFormat("Energy: %.1f", andreas.energy), 10, 10 + dynamicFontSize + 10, dynamicFontSize, DARKGREEN);
            DrawText(TextFormat("Stress: %.1f", andreas.stress), 10, 10 + (dynamicFontSize * 2) + 20, dynamicFontSize, RED);

            const char* footer = "Andreas is exploring CCU campus...";
            int footerWidth = MeasureText(footer, dynamicFontSize);
            DrawText(footer, (screenWidth/2) - (footerWidth/2), screenHeight - (dynamicFontSize*2), dynamicFontSize, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}