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

// Tambahkan MINI_MISSION di dalam Enum Screens
typedef enum { MINI_NONE = 0, MINI_MATH, MINI_MANDARIN, MINI_ENGLISH, MINI_MISSION, MINI_RESULT } MiniScreen;

typedef struct { float timeLeft; int score; int total; } TaskBase;

typedef struct {
    TaskBase base; int a; int b; int op; int correct;
    char question[64]; char input[MAX_INPUT]; int inputLen;
} MathGame;

typedef struct { const char *pinyin; const char *meaning; const char *choices[4]; int correctIndex; } MandarinQuestion;
typedef struct { TaskBase base; int current; } MandarinGame;

typedef struct { const char *answer; const char *scramble; const char *hint; } EnglishWord;
typedef struct { TaskBase base; int current; char input[MAX_INPUT]; int inputLen; } EnglishGame;

// ── STRUCT BARU UNTUK GENERAL SUBJECT QUIZ ──
typedef struct {
    const char *question;
    const char *options[4];
    int correctAnswer;
} MissionQuestion;

typedef enum { MISSION_PLAYING = 0, MISSION_FEEDBACK, MISSION_DONE } MissionPhase;

typedef struct {
    TaskBase     base;
    MissionPhase phase;
    int  currentQuestion;
    int  selectedOption;
    int  correctCount;
    int  wrongCount;
    bool isCorrect;
    float feedbackTimer;
} MissionGame;


// Variabel Global Modul Mini-Game
extern MiniScreen currentMiniScreen;
extern MathGame mathGame;
extern MandarinGame mandarinGame;
extern EnglishGame englishGame;
extern MissionGame missionGame; // <-- Tambahkan extern ini agar bisa dibaca main.c
extern char lastGameTitle[64];
extern int lastScore;
extern int lastTotal;
extern int triggerMissionType;

static MandarinQuestion mandarinQuestions[] = {
  {"ni hao", "hello", {"thank you", "hello", "goodbye", "teacher"}, 1},
  {"xie xie", "thank you", {"sorry", "thank you", "student", "water"}, 1},
  {"zai jian", "goodbye", {"friend", "today", "goodbye", "rice"}, 2},
  {"qing", "please", {"please", "tomorrow", "school", "fish"}, 0},
  {"dui bu qi", "sorry", {"teacher", "sorry", "hello", "market"}, 1},
  {"tijiao", "To submit", {"To submit", "To borrow", "To sleep", "To forget"}, 0},
  {"quexi", "To be absent", {"To attend", "To be absent", "To review", "To answer"}, 1},
  {"chidao", "To be late", {"To be early", "To be late", "To finish", "To prepare"}, 1},
  {"zhunbei", "To prepare", {"To prepare", "To borrow", "To clean", "To choose"}, 0},
  {"fuxi", "To review", {"To preview", "To review", "To rest", "To print"}, 1},
  {"yuxi", "To preview", {"To preview", "To practice", "To discuss", "To explain"}, 0},
  {"taolun", "To discuss", {"To explain", "To discuss", "To memorize", "To register"}, 1},
  {"jieshi", "To explain", {"To explain", "To submit", "To answer", "To fail"}, 0},
  {"chengji", "Grade", {"Attendance", "Grade", "Schedule", "Course"}, 1},
  {"biye", "To graduate", {"To enroll", "To graduate", "To transfer", "To register"}, 1},
  {"qixian", "deadline", {"deadline", "grade", "course", "notes"}, 0},
  {"chuxi", "attendance", {"attendance", "absence", "late", "submit"}, 0},
  {"baogao", "presentation", {"presentation", "exam", "homework", "rest"}, 0},
  {"taolun", "discussion", {"discussion", "preview", "review", "class"}, 0},
  {"shiyan", "experiment", {"experiment", "textbook", "classroom", "project"}, 0},
  {"xiangmu", "project", {"project", "exam", "grade", "semester"}, 0},
  {"xueqi", "Semester", {"Semester", "Classroom", "Department", "Professor"}, 0},
  {"xisuo", "Department", {"University", "Department", "Cafeteria", "Library"}, 1},
  {"jiangxuejin", "Scholarship", {"Scholarship", "Assignment", "Examination", "Registration"}, 0},
  {"baoming", "To register", {"To graduate", "To register", "To review", "To explain"}, 1},
  {"jiangxuejin", "scholarship", {"scholarship", "student ID card", "library", "research room"}, 0},
  {"xueshengzheng", "student ID card", {"student ID card", "ID card", "department", "course"}, 0},
  {"yanjiushi", "Research room", {"Office", "Research room", "Gymnasium", "Dormitory"}, 1},
  {"jiandao", "To check in", {"To check in", "To submit", "To graduate", "To be absent"}, 0},
  {"xiaoyuan huodong", "campus activity", {"campus activity", "semester grade", "course deadline", "lab report"}, 0}

};

static EnglishWord englishWords[] = {
    {"fish", "hsif", "Animal that lives in water"},
    {"water", "trewa", "You drink it"},
    {"school", "ohoscl", "Place for learning"},
    {"computer", "teromcpu", "smart and multifunctional machine"},
    {"assignment", "tnemngissa", "A difficult school task"},
    {"dormitory", "yrotimrod", "A place where students live"},
    {"professor", "rosseforp", "A person who teaches at university"},
    {"laboratory", "yrotarobal", "A place for experiments or coding practice"},
    {"examination", "noitanimaxe", "A written test"},
    {"schedule", "eludehcs", "A list of class times"},
    {"course", "esruoc", "A school subject"},
    {"student", "tneduts", "A person studying at university"},
    {"campus", "supmac", "A school building area"},
    {"cafeteria", "airetefac", "A place to eat food"},
    {"quiz", "ziuq", "A short test"},
    {"assistant", "tnatsissa", "A teacher’s helper"},
    {"instruction", "noitcurtsni", "A thing students must follow"},
    {"classmate", "etamssalc", "A person in the same class"},
    {"gymnasium", "muisanmyg", "A place for sports"},
    {"programming", "gnimmargorp", "A subject about computers"},
    {"evaluation", "noitaulave", "A result or grade"},
    {"project", "tcejorp", "A final school work"},
    {"regulation", "noitaluger", "A rule students must obey"},
    {"mission", "noissim", "A goal or task in a game"}
};

// ── DATA PERTANYAAN BARU UNTUK GENERAL SUBJECT QUIZ ──
#define MAX_MISSION_QUESTIONS 5
static MissionQuestion missionQuestions[MAX_MISSION_QUESTIONS] = {
    {"What is the capital city of Indonesia?", {"Bandung", "Jakarta", "Surabaya", "Medan"}, 1},
    {"Which one is a good student habit?", {"Cheating", "Studying regularly", "Skipping class", "Ignoring homework"}, 1},
    {"What language is used for international communication?", {"English", "Javanese", "Sundanese", "Balinese"}, 0},
    {"Which action helps protect the environment?", {"Throwing trash anywhere", "Saving water", "Burning plastic", "Cutting trees carelessly"}, 1},
    {"What should we do before using info from the internet?", {"Copy it directly", "Check the source", "Ignore the author", "Delete the reference"}, 1}
};

static inline int MandarinCount(void) { return 30;}
static inline int EnglishCount(void) { return 24; }

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

// ── FUNGSI START BARU UNTUK MISSION GAME ──
static inline void StartMissionGame(void) {
    ResetBase(&missionGame.base);
    missionGame.base.timeLeft   = TASK_TIME * 2.0f; // Diberi waktu 60 detik (karena membaca soal butuh waktu lebih lama)
    missionGame.phase           = MISSION_PLAYING;
    missionGame.currentQuestion = 0;
    missionGame.selectedOption  = 0;
    missionGame.correctCount    = 0;
    missionGame.wrongCount      = 0;
    missionGame.isCorrect       = false;
    missionGame.feedbackTimer   = 0.0f;
    currentMiniScreen           = MINI_MISSION;
}

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
    // Batasi barWidth agar tidak menggambar bar minus/melebihi batas jika waktu dikali 2
    if (barWidth > 1220.0f) barWidth = 1220.0f;
    DrawRectangle(30, 86, 1220, 12, LIGHTGRAY);
    DrawRectangle(30, 86, (int)barWidth, 12, SKYBLUE);
}

static inline void DrawButtonBox(int x, int y, int w, int h, const char *label, Color color) {
    DrawRectangleRounded((Rectangle){x, y, w, h}, 0.2f, 8, color);
    DrawRectangleRoundedLines((Rectangle){x, y, w, h}, 0.2f, 8, DARKGRAY);
    DrawText(label, x + 25, y + 20, 24, BLACK);
}

#endif