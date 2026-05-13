#include "raylib.h"
#include <stdlib.h>
#include <time.h>

// --- BAGIAN 1: HEADER & STRUKTUR DATA ---
#define TILE_SIZE 30
#define MAP_WIDTH 25
#define MAP_HEIGHT 25

typedef struct {
    int x, y;
} Position;

typedef struct {
    char **grid;
    int w, h;
} Map;

// --- BAGIAN 2: LOGIKA REKURSI JOSES (MAZE GENERATOR) ---
void generateMaze(Map *m, int y, int x) {
    int dirs[4][2] = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}};
    // Acak arah
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int ty = dirs[r][1], tx = dirs[r][0];
        dirs[r][1] = dirs[i][1]; dirs[r][0] = dirs[i][0];
        dirs[i][1] = ty; dirs[i][0] = tx;
    }

    for (int i = 0; i < 4; i++) {
        int ny = y + dirs[i][1];
        int nx = x + dirs[i][0];

        if (ny > 0 && ny < m->h - 1 && nx > 0 && nx < m->w - 1 && m->grid[ny][nx] == '#') {
            m->grid[y + dirs[i][1] / 2][x + dirs[i][0] / 2] = ' '; 
            m->grid[ny][nx] = ' ';
            generateMaze(m, ny, nx);
        }
    }
}

// --- BAGIAN 3: MAIN PROGRAM (INTEGRASI SEMUA) ---
int main() {
    srand(time(NULL));
    
    // Inisialisasi Window Raylib
    InitWindow(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE + 50, "CSIE CCU - Joses's Dungeon");
    SetTargetFPS(60);

    // DYNAMIC ALLOCATION (Materi Minggu 4)
    Map m;
    m.w = MAP_WIDTH; m.h = MAP_HEIGHT;
    m.grid = (char **)malloc(m.h * sizeof(char *));
    for (int i = 0; i < m.h; i++) {
        m.grid[i] = (char *)malloc(m.w * sizeof(char));
        for (int j = 0; j < m.w; j++) m.grid[i][j] = '#';
    }

    // Jalankan Algoritma Joses
    m.grid[1][1] = ' ';
    generateMaze(&m, 1, 1);

    // Posisi Player
    Position player = {1, 1};

    // GAME LOOP
    while (!WindowShouldClose()) {
        // INPUT: Pergerakan Player
        if (IsKeyPressed(KEY_UP) && m.grid[player.y - 1][player.x] == ' ') player.y--;
        if (IsKeyPressed(KEY_DOWN) && m.grid[player.y + 1][player.x] == ' ') player.y++;
        if (IsKeyPressed(KEY_LEFT) && m.grid[player.y][player.x - 1] == ' ') player.x--;
        if (IsKeyPressed(KEY_RIGHT) && m.grid[player.y][player.x + 1] == ' ') player.x++;

        // DRAWING
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < m.h; i++) {
            for (int j = 0; j < m.w; j++) {
                if (m.grid[i][j] == '#') 
                    DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE - 2, TILE_SIZE - 2, DARKGRAY);
                else 
                    DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE - 2, TILE_SIZE - 2, RAYWHITE);
            }
        }

        // Gambar Player (Biru)
        DrawRectangle(player.x * TILE_SIZE, player.y * TILE_SIZE, TILE_SIZE - 2, TILE_SIZE - 2, BLUE);
        
        DrawText("Use Arrows to Move | Press ESC to Exit", 10, MAP_HEIGHT * TILE_SIZE + 15, 20, WHITE);
        EndDrawing();
    }

    // CLEANUP: Free Memory
    for (int i = 0; i < m.h; i++) free(m.grid[i]);
    free(m.grid);
    CloseWindow();

    return 0;
}