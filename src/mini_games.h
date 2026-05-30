#ifndef MINI_GAMES_H
#define MINI_GAMES_H

#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MINI_VIRTUAL_WIDTH 1280
#define MINI_VIRTUAL_HEIGHT 720
#define TASK_TIME 30.0f
#define MAX_INPUT 32

typedef enum { MINI_NONE = 0, MINI_MATH, MINI_MANDARIN, MINI_ENGLISH, MINI_RESULT } MiniScreen;

typedef struct { float timeLeft; int score; int total; } TaskBase;

typedef struct {
    TaskBase base; int a; int b; int op; int correct;
    char question[64]; char input[MAX_INPUT]; int inputLen;
} MathGame;

typedef struct { const char *pinyin; const char *meaning; const char *choices[4]; int correctIndex; } MandarinQuestion;
typedef struct { TaskBase base; int current; } MandarinGame;

typedef struct { const char *answer; const char *scramble; const char *hint; } EnglishWord;
typedef struct { TaskBase base; int current; char input[MAX_INPUT]; int inputLen; } EnglishGame;

// Variabel Global Modul Mini-Game
extern MiniScreen currentMiniScreen;
extern MathGame mathGame;
extern MandarinGame mandarinGame;
extern EnglishGame englishGame;
extern char lastGameTitle[64];
extern int lastScore;
extern int lastTotal;
extern int triggerMissionType;

static MandarinQuestion mandarinQuestions[] = {
    {"ni hao", "hello", {"thank you", "hello", "goodbye", "teacher"}, 1},
    {"xie xie", "thank you", {"sorry", "thank you", "student", "water"}, 1},
    {"zai jian", "goodbye", {"friend", "today", "goodbye", "rice"}, 2},
    {"qing", "please", {"please", "tomorrow", "school", "fish"}, 0},
    {"dui bu qi", "sorry", {"teacher", "sorry", "hello", "market"}, 1}
};

static EnglishWord englishWords[] = {
    {"fish", "hsif", "Animal that lives in water"},
    {"water", "trewa", "You drink it"},
    {"school", "ohoscl", "Place for learning"},
    {"computer", "teromcpu", "smart and multifunctional machine"}
};

static inline int MandarinCount(void) { return 5; }
static inline int EnglishCount(void) { return 4; }

static inline void ResetBase(TaskBase *base) {
    base->timeLeft = TASK_TIME;
    base->score = 0;
    base->total = 0;
}

static inline int EqualsIgnoreCase(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

static inline void FinishTask(const char *title, TaskBase base) {
    strncpy(lastGameTitle, title, sizeof(lastGameTitle) - 1);
    lastScore = base.score;
    lastTotal = base.total;
    currentMiniScreen = MINI_RESULT;
}

static inline bool UpdateTimer(TaskBase *base, const char *title, float dt) {
    base->timeLeft -= dt;
    if (base->timeLeft <= 0.0f) {
        base->timeLeft = 0.0f;
        FinishTask(title, *base);
        return true;
    }
    return false;
}

static inline void GenerateMathQuestion(void) {
    mathGame.op = rand() % 3;
    if (mathGame.op == 0) {
        mathGame.a = 1 + rand() % 50; mathGame.b = 1 + rand() % 50;
        mathGame.correct = mathGame.a + mathGame.b;
        snprintf(mathGame.question, sizeof(mathGame.question), "%d + %d = ?", mathGame.a, mathGame.b);
    } else if (mathGame.op == 1) {
        mathGame.a = 10 + rand() % 60; mathGame.b = 1 + rand() % mathGame.a;
        mathGame.correct = mathGame.a - mathGame.b;
        snprintf(mathGame.question, sizeof(mathGame.question), "%d - %d = ?", mathGame.a, mathGame.b);
    } else {
        mathGame.a = 1 + rand() % 12; mathGame.b = 1 + rand() % 12;
        mathGame.correct = mathGame.a * mathGame.b;
        snprintf(mathGame.question, sizeof(mathGame.question), "%d x %d = ?", mathGame.a, mathGame.b);
    }
    mathGame.input[0] = '\0'; mathGame.inputLen = 0;
}

static inline void StartMathGame(void) { ResetBase(&mathGame.base); GenerateMathQuestion(); currentMiniScreen = MINI_MATH; }
static inline void StartMandarinGame(void) { ResetBase(&mandarinGame.base); mandarinGame.current = rand() % MandarinCount(); currentMiniScreen = MINI_MANDARIN; }
static inline void StartEnglishGame(void) { ResetBase(&englishGame.base); englishGame.current = rand() % EnglishCount(); englishGame.input[0] = '\0'; englishGame.inputLen = 0; currentMiniScreen = MINI_ENGLISH; }

static inline void AddInputChar(char *input, int *inputLen, int ch, bool mathMode) {
    if (*inputLen >= MAX_INPUT - 1) return;
    if (mathMode) {
        if (isdigit(ch) || (ch == '-' && *inputLen == 0)) { input[*inputLen] = (char)ch; (*inputLen)++; input[*inputLen] = '\0'; }
    } else {
        if (isalpha(ch)) { input[*inputLen] = (char)tolower(ch); (*inputLen)++; input[*inputLen] = '\0'; }
    }
}

static inline void BackspaceInput(char *input, int *inputLen) { if (*inputLen > 0) { (*inputLen)--; input[*inputLen] = '\0'; } }

static inline void DrawCenteredText(const char *text, int y, int fontSize, Color color) {
    int width = MeasureText(text, fontSize);
    DrawText(text, MINI_VIRTUAL_WIDTH / 2 - width / 2, y, fontSize, color);
}

static inline void DrawHeader(const char *title, TaskBase base) {
    DrawRectangle(0, 0, MINI_VIRTUAL_WIDTH, 80, (Color){30, 40, 55, 255});
    DrawText(title, 30, 24, 30, RAYWHITE);
    DrawText(TextFormat("Time: %02d", (int)base.timeLeft), 1000, 20, 26, RAYWHITE);
    DrawText(TextFormat("Score: %d/%d", base.score, base.total), 1000, 50, 20, LIGHTGRAY);
    float barWidth = 1220.0f * (base.timeLeft / TASK_TIME);
    DrawRectangle(30, 86, 1220, 12, LIGHTGRAY);
    DrawRectangle(30, 86, (int)barWidth, 12, SKYBLUE);
}

static inline void DrawButtonBox(int x, int y, int w, int h, const char *label, Color color) {
    DrawRectangleRounded((Rectangle){x, y, w, h}, 0.2f, 8, color);
    DrawRectangleRoundedLines((Rectangle){x, y, w, h}, 0.2f, 8, DARKGRAY);
    DrawText(label, x + 25, y + 20, 24, BLACK);
}

#endif