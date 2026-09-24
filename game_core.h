#ifndef GAME_CORE_H
#define GAME_CORE_H

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996)

#include "iGraphics.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma comment(lib, "winmm.lib")

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ==================== CONSTANTS ====================
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define FPS 50
#define TIMER_INTERVAL_MS 20
#define MAX_LOOP_DURATION_SEC 90
#define MAX_FRAMES (FPS * MAX_LOOP_DURATION_SEC)
#define MAX_PLATFORMS 35
#define MAX_SWITCHES 8
#define MAX_LASERS 8
#define MAX_PARTICLES 200
#define TOTAL_LEVELS 5
#define MAX_ENEMIES 3
#define MAX_BULLETS 40
#define MAX_BIRDS 5
#define MAX_CLONES 8

// Game States
#define STATE_MENU 0
#define STATE_LEVEL_SELECT 1
#define STATE_PLAYING 2
#define STATE_PARADOX_RESET 3
#define STATE_LOOP_REWIND 4
#define STATE_LEVEL_CLEAR 5
#define STATE_INSTRUCTIONS 6
#define STATE_CREDITS 7
#define STATE_GAME_VICTORY 8
#define STATE_CHARACTER_SELECT 9

// Animation States
#define ANIM_IDLE 0
#define ANIM_RUN 1
#define ANIM_JUMP 2
#define ANIM_FALL 3

// Platform Types
#define PLATFORM_SOLID 0
#define PLATFORM_MOVING_H 1
#define PLATFORM_MOVING_V 2

// ==================== CHARACTER ====================
#define TOTAL_CHARACTERS 2
const char* CHARACTER_FOLDERS[TOTAL_CHARACTERS] = { "GOAT", "HERO" };
int selectedCharacter = 0;
int characterIdleRight[TOTAL_CHARACTERS][6];
int characterIdleLeft[TOTAL_CHARACTERS][6];
int characterRunRight[TOTAL_CHARACTERS][8];
int characterRunLeft[TOTAL_CHARACTERS][8];
int characterJumpUp[TOTAL_CHARACTERS][6];
int characterFall[TOTAL_CHARACTERS][6];
int characterLand[TOTAL_CHARACTERS][6];
int birdSprite[6];
int enemyWalkRight[8];
int enemyWalkLeft[8];
int enemyShootRight[8];
int enemyShootLeft[8];
int platformGrassImg = -1;

// ==================== STRUCTURES ====================
struct FrameSnapshot {
	float x, y, vx, vy;
	int animState;
	bool facingRight;
	bool isGrounded;
	int animFrame;
};

struct Player {
	float x, y, vx, vy;
	float width, height;
	bool isGrounded, facingRight;
	int animState, animFrame, coyoteTimer, jumpBufferTimer, hitCooldown;
	float animTimer;
};

struct Clone {
	bool isActive;
	float x, y, vx, vy, width, height;
	bool facingRight;
	int animState, animFrame, totalRecordedFrames;
	bool hasSeparated;
	FrameSnapshot history[MAX_FRAMES];
};

struct Platform {
	float x, y, w, h;
	int type;
	float startX, startY, minBound, maxBound, speed;
	int direction, r, g, b;
};

struct Switch {
	float x, y, w, h;
	int switchId;
	bool isPressed;
	int targetLaserId;
	bool isFake;
};

struct LaserGate {
	float x, y, w, h;
	int laserId;
	bool isActive, isVertical;
	float pulsePhase;
};

struct ExitPortal {
	float x, y, radius;
	bool isUnlocked;
	float rotationAngle, pulse;
};

struct Enemy {
	float x, y, w, h, minX, maxX, speed, bulletSpeed;
	int direction, fireTimer, fireInterval;
};

struct Bullet {
	float x, y, w, h, vx, vy;
	bool active;
};

struct Bird {
	float x, y, spawnX, spawnY, vx, vy, speed, wobblePhase, bulletSpeed, w, h;
	int fireTimer, fireInterval;
};

struct Particle {
	float x, y, vx, vy, life, maxLife, size;
	int r, g, b;
	bool active;
};

struct LevelData {
	char name[64], subtitle[128];
	float spawnX, spawnY;
	ExitPortal portal;
	int loopDurationFrames, platformCount, switchCount, laserCount, enemyCount, birdCount;
	Platform platforms[MAX_PLATFORMS];
	Switch switches[MAX_SWITCHES];
	LaserGate lasers[MAX_LASERS];
	Enemy enemies[MAX_ENEMIES];
	Bird birds[MAX_BIRDS];
};

// Score Structure (Text File Database)
struct GameScore {
	int currentScore;
	int highestScore;
	int levelsCleared;
};

// ==================== GLOBALS ====================
int gameState = STATE_MENU;
int currentLevelIndex = 0;
int loopsUsedInLevel = 1;
int totalParadoxCount = 0;
bool levelUnlocked[TOTAL_LEVELS] = { true, false, false, false, false };
int levelBackgroundID[TOTAL_LEVELS];
int menuBackgroundID = -1, levelSelectBackgroundID = -1, instructionsBackgroundID = -1, creditsBackgroundID = -1;
Player player;
Clone clones[MAX_CLONES];
int activeCloneCount = 0;
FrameSnapshot currentRecording[MAX_FRAMES];
int currentFrameTick = 0;
int timePenaltyFrames = 0;
int paradoxAnimationTimer = 0;
int rewindAnimationTimer = 0;
float screenShakeOffset = 0.0f;
float globalAnimTimer = 0.0f;
Particle particles[MAX_PARTICLES];
LevelData levels[TOTAL_LEVELS];
Bullet bullets[MAX_BULLETS];
GameScore gameScore;
bool prevRewindKey = false, prevEscKey = false, prevJumpKey = false, prevVolUpKey = false, prevVolDownKey = false;
int mouseX = 0, mouseY = 0;
int musicVolume = 600;
int musicCheckTimer = 0;
bool menuMusicActive = false;

// ==================== FORWARD DECLARATIONS ====================
void initAllLevels();
void loadLevel(int levelIdx);
void loadAllCharacterSprites();
void loadAllEnemySprites();
void resetLoop(bool isParadox);
void spawnParticle(float x, float y, float vx, float vy, int r, int g, int b, float size, float life);
void updateParticles();
void drawParticles();
void triggerSound(const char* soundFile);
bool checkAABB(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
void spawnBullet(float x, float y, float vx, float vy);
void clearAllBullets();
void playBackgroundMusic(const char* filepath);
void updateBackgroundMusicLoop();
void setMusicVolume(int newVolume);
void goToMenu();
void triggerSoundMp3(const char* filepath);
void triggerParadoxWarning();
void saveGameData();
void loadGameData();
void updateScore();

// ==================== HELPER FUNCTIONS ====================
void triggerSound(const char* soundFile) {
	PlaySound(soundFile, NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
}

void triggerSoundMp3(const char* filepath) {
	mciSendString("close uiClick", NULL, 0, NULL);
	char cmd[256];
	sprintf(cmd, "open \"%s\" type MPEGVideo alias uiClick", filepath);
	mciSendString(cmd, NULL, 0, NULL);
	mciSendString("play uiClick from 0", NULL, 0, NULL);
}

void triggerParadoxWarning() {
	triggerSound("sounds/paradox_alert.wav");
}

bool checkAABB(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

const char* MENU_MUSIC_FILE = "music/menu_theme.mp3";
const char* LEVEL_MUSIC_FILE[TOTAL_LEVELS] = {
	"music/level1_theme.mp3", "music/level2_theme.mp3", "music/level3_theme.mp3",
	"music/level4_theme.mp3", "music/level5_theme.mp3"
};

void playBackgroundMusic(const char* filepath) {
	mciSendString("close bgMusic", NULL, 0, NULL);
	char cmd[256];
	sprintf(cmd, "open \"%s\" type MPEGVideo alias bgMusic", filepath);
	mciSendString(cmd, NULL, 0, NULL);
	sprintf(cmd, "setaudio bgMusic volume to %d", musicVolume);
	mciSendString(cmd, NULL, 0, NULL);
	mciSendString("play bgMusic from 0", NULL, 0, NULL);
	musicCheckTimer = 0;
}

void updateBackgroundMusicLoop() {
	musicCheckTimer++;
	if (musicCheckTimer < 5) return;
	musicCheckTimer = 0;
	char lengthBuf[64] = "", posBuf[64] = "";
	mciSendString("status bgMusic length", lengthBuf, sizeof(lengthBuf), NULL);
	mciSendString("status bgMusic position", posBuf, sizeof(posBuf), NULL);
	long length = atol(lengthBuf);
	long pos = atol(posBuf);
	if (length > 0 && pos >= length - 100) {
		mciSendString("play bgMusic from 0", NULL, 0, NULL);
	}
}

void setMusicVolume(int newVolume) {
	if (newVolume < 0) newVolume = 0;
	if (newVolume > 1000) newVolume = 1000;
	musicVolume = newVolume;
	char cmd[128];
	sprintf(cmd, "setaudio bgMusic volume to %d", musicVolume);
	mciSendString(cmd, NULL, 0, NULL);
}

void goToMenu() {
	gameState = STATE_MENU;
	if (!menuMusicActive) {
		playBackgroundMusic(MENU_MUSIC_FILE);
		menuMusicActive = true;
	}
}

void spawnParticle(float x, float y, float vx, float vy, int r, int g, int b, float size, float life) {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (!particles[i].active) {
			particles[i].active = true;
			particles[i].x = x; particles[i].y = y;
			particles[i].vx = vx; particles[i].vy = vy;
			particles[i].r = r; particles[i].g = g; particles[i].b = b;
			particles[i].size = size; particles[i].life = life; particles[i].maxLife = life;
			return;
		}
	}
}

void updateParticles() {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (particles[i].active) {
			particles[i].x += particles[i].vx;
			particles[i].y += particles[i].vy;
			particles[i].life -= 0.02f;
			if (particles[i].life <= 0) particles[i].active = false;
		}
	}
}

void drawParticles() {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (particles[i].active) {
			float alpha = particles[i].life / particles[i].maxLife;
			if (alpha < 0) alpha = 0;
			iSetColor(particles[i].r, particles[i].g, (int)(particles[i].b * alpha));
			iFilledCircle(particles[i].x + screenShakeOffset, particles[i].y, particles[i].size * alpha, 8);
		}
	}
}

void spawnBullet(float x, float y, float vx, float vy) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (!bullets[i].active) {
			bullets[i].active = true;
			bullets[i].x = x; bullets[i].y = y;
			bullets[i].w = 10; bullets[i].h = 5;
			bullets[i].vx = vx; bullets[i].vy = vy;
			return;
		}
	}
}

void clearAllBullets() {
	for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = false;
}

// ==================== SAVE / LOAD (Text File Database) ====================
void saveGameData() {
	FILE* fp = fopen("savegame.txt", "w");
	if (fp == NULL) return;
	fprintf(fp, "%d %d\n", gameScore.highestScore, gameScore.levelsCleared);
	for (int i = 0; i < TOTAL_LEVELS; i++) {
		fprintf(fp, "%d ", levelUnlocked[i] ? 1 : 0);
	}
	fprintf(fp, "\n");
	fclose(fp);
}

void loadGameData() {
	FILE* fp = fopen("savegame.txt", "r");
	if (fp == NULL) {
		gameScore.highestScore = 0;
		gameScore.levelsCleared = 0;
		gameScore.currentScore = 0;
		levelUnlocked[0] = true;
		for (int i = 1; i < TOTAL_LEVELS; i++) levelUnlocked[i] = false;
		return;
	}
	fscanf(fp, "%d %d", &gameScore.highestScore, &gameScore.levelsCleared);
	for (int i = 0; i < TOTAL_LEVELS; i++) {
		int val;
		fscanf(fp, "%d", &val);
		levelUnlocked[i] = (val == 1);
	}
	fclose(fp);

	// ← এই লাইনটা যোগ করো
	levelUnlocked[0] = true;          // Level 1 সবসময় আনলক থাকবে

	gameScore.currentScore = 0;
}
void updateScore() {
	// বেস স্কোর: প্রতি লেভেল ক্লিয়ার করার জন্য
	int baseScore = (currentLevelIndex + 1) * 300;

	// সময় বোনাস: যত কম লুপ ব্যবহার করবে, তত বেশি পয়েন্ট
	// (loopsUsedInLevel কম হলে বেশি স্কোর)
	int timeBonus = 800 - (loopsUsedInLevel * 60);
	if (timeBonus < 0) timeBonus = 0;

	// প্যারাডক্স পেনাল্টি
	int paradoxPenalty = totalParadoxCount * 40;

	int score = baseScore + timeBonus - paradoxPenalty;
	if (score < 0) score = 0;

	gameScore.currentScore = score;

	// Highest Score আপডেট
	if (gameScore.currentScore > gameScore.highestScore) {
		gameScore.highestScore = gameScore.currentScore;
	}
}

// ==================== LEVEL DATA ====================
void initAllLevels() {
	// LEVEL 1
	LevelData* l1 = &levels[0];
	sprintf(l1->name, "LEVEL 1: FIRST ECHO");
	sprintf(l1->subtitle, "Stand on the switch, rewind (R), then sprint to the exit!");
	l1->spawnX = 100; l1->spawnY = 134;
	l1->loopDurationFrames = FPS * 35;
	l1->portal = { 1158, 178, 35, true, 0, 0 };
	l1->platformCount = 5;
	l1->platforms[0] = { 0, 0, 1200, 100, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l1->platforms[1] = { 0, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l1->platforms[2] = { 1160, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l1->platforms[3] = { 200, 150, 200, 25, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l1->platforms[4] = { 500, 180, 150, 25, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l1->switchCount = 1;
	l1->switches[0] = { 280, 175, 40, 10, 0, false, 0, false };
	l1->laserCount = 1;
	l1->lasers[0] = { 900, 100, 20, 500, 0, true, true, 0 };
	l1->enemyCount = 0; l1->birdCount = 0;

	// LEVEL 2
	LevelData* l2 = &levels[1];
	sprintf(l2->name, "LEVEL 2: DUAL BARRIER");
	sprintf(l2->subtitle, "Use your clone to hold one switch while you cross the other gate!");
	l2->spawnX = 80; l2->spawnY = 120;
	l2->loopDurationFrames = FPS * 40;
	l2->portal = { 1100, 480, 35, true, 0, 0 };
	l2->platformCount = 7;
	l2->platforms[0] = { 0, 0, 1200, 100, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l2->platforms[1] = { 0, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l2->platforms[2] = { 1160, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l2->platforms[3] = { 150, 150, 260, 25, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l2->platforms[4] = { 550, 180, 260, 25, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l2->platforms[5] = { 950, 390, 210, 25, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l2->platforms[6] = { 700, 300, 380, 25, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l2->switchCount = 2;
	l2->switches[0] = { 250, 175, 40, 10, 0, false, 0, false };
	l2->switches[1] = { 800, 325, 40, 10, 1, false, 1, false };
	l2->laserCount = 2;
	l2->lasers[0] = { 450, 100, 20, 220, 0, true, true, 0 };
	l2->lasers[1] = { 920, 360, 20, 250, 1, true, true, 0 };
	l2->enemyCount = 0;
	l2->birdCount = 1;
	l2->birds[0] = { 150, 620, 150, 620, 0, 0, 1.05f, 0.0f, 4.0f, 40, 28, 0, 180 };

	// LEVEL 3
	LevelData* l3 = &levels[2];
	sprintf(l3->name, "LEVEL 3: VERTICAL RIFT");
	sprintf(l3->subtitle, "Coordinate multiple switches with clones!");
	l3->spawnX = 80; l3->spawnY = 120;
	l3->loopDurationFrames = FPS * 45;
	l3->portal = { 1080, 470, 35, true, 0, 0 };
	l3->platformCount = 10;
	l3->platforms[0] = { 0, 0, 1200, 100, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l3->platforms[1] = { 0, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l3->platforms[2] = { 1160, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l3->platforms[3] = { 140, 180, 240, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l3->platforms[4] = { 420, 180, 200, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l3->platforms[5] = { 680, 220, 200, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l3->platforms[6] = { 200, 280, 220, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l3->platforms[7] = { 500, 320, 180, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l3->platforms[8] = { 780, 380, 180, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l3->platforms[9] = { 1000, 450, 160, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l3->switchCount = 4;
	l3->switches[0] = { 220, 202, 40, 10, 0, false, 0, false };
	l3->switches[1] = { 280, 302, 40, 10, 1, false, 1, false };
	l3->switches[2] = { 740, 242, 40, 10, 2, false, 2, false };
	l3->switches[3] = { 560, 342, 40, 10, 3, false, 3, false };
	l3->laserCount = 4;
	l3->lasers[0] = { 380, 100, 18, 120, 0, true, true, 0 };
	l3->lasers[1] = { 700, 100, 18, 280, 1, true, true, 0 };
	l3->lasers[2] = { 920, 100, 18, 360, 2, true, true, 0 };
	l3->lasers[3] = { 600, 100, 18, 240, 3, true, true, 0 };
	l3->enemyCount = 1;
	l3->enemies[0] = { 750, 100, 30, 34, 400, 750, 1.1f, -1, 0, 160};
	l3->birdCount = 1;
	l3->birds[0] = { 200, 620, 200, 620, 0, 0, 1.1f, 0.0f, 4.2f, 30, 22, 0, 170 };
	// LEVEL 4
	LevelData* l4 = &levels[3];
	sprintf(l4->name, "LEVEL 4: PARADOX LEAP");
	sprintf(l4->subtitle, "Watch out for Fake Switches!");
	l4->spawnX = 70; l4->spawnY = 120;
	l4->loopDurationFrames = FPS * 80;
	l4->portal = { 1100, 310, 35, true, 0, 0 };
	l4->platformCount = 11;
	l4->platforms[0] = { 0, 0, 1200, 100, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l4->platforms[1] = { 0, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l4->platforms[2] = { 1160, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l4->platforms[3] = { 80, 170, 170, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l4->platforms[4] = { 310, 190, 170, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l4->platforms[5] = { 540, 170, 170, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l4->platforms[6] = { 770, 190, 170, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l4->platforms[7] = { 200, 290, 180, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l4->platforms[8] = { 980, 280, 160, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l4->platforms[9] = { 420, 290, 150, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l4->platforms[10] = { 650, 290, 150, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 50, 70, 100 };
	l4->switchCount = 6;
	l4->switches[0] = { 130, 192, 40, 10, 0, false, 0, false };
	l4->switches[1] = { 360, 212, 40, 10, 1, false, 1, false };
	l4->switches[2] = { 590, 192, 40, 10, 2, false, 2, false };
	l4->switches[3] = { 820, 212, 40, 10, 3, false, 3, false };
	l4->switches[4] = { 470, 312, 40, 10, 4, false, -1, true };
	l4->switches[5] = { 700, 312, 40, 10, 5, false, -1, true };
	l4->laserCount = 4;
	l4->lasers[0] = { 260, 100, 18, 150, 0, true, true, 0 };
	l4->lasers[1] = { 490, 100, 18, 170, 1, true, true, 0 };
	l4->lasers[2] = { 720, 100, 18, 170, 2, true, true, 0 };
	l4->lasers[3] = { 960, 180, 18, 160, 3, true, true, 0 };
	l4->enemyCount = 1;
	l4->enemies[0] = { 400, 100, 30, 34, 200, 400, 1.15f, 4.5f, -1, 0, 150 };
	l4->birdCount = 2;
	l4->birds[0] = { 150, 620, 150, 620, 0, 0, 1.15f, 0.0f, 4.3f, 30, 22, 0, 160 };
	l4->birds[1] = { 1050, 620, 1050, 620, 0, 0, 1.15f, 1.2f, 4.3f, 30, 22, 0, 160 };
	// LEVEL 5
	LevelData* l5 = &levels[4];
	sprintf(l5->name, "LEVEL 5: DRAGON HALL");
	sprintf(l5->subtitle, "Final Level! Beware the dragons!");
	l5->spawnX = 80; l5->spawnY = 120;
	l5->loopDurationFrames = FPS * 80;
	l5->portal = { 1100, 480, 35, true, 0, 0 };
	l5->platformCount = 14;
	l5->platforms[0] = { 0, 0, 1200, 100, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l5->platforms[1] = { 0, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l5->platforms[2] = { 1160, 100, 40, 600, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 30, 40, 60 };
	l5->platforms[3] = { 100, 160, 160, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 70, 60, 55 };
	l5->platforms[4] = { 320, 200, 150, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 70, 60, 55 };
	l5->platforms[5] = { 530, 240, 150, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 70, 60, 55 };
	l5->platforms[6] = { 740, 280, 150, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 70, 60, 55 };
	l5->platforms[7] = { 950, 340, 140, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 70, 60, 55 };
	l5->platforms[8] = { 1040, 450, 120, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 70, 60, 55 };
	l5->platforms[9] = { 200, 280, 80, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 80, 70, 65 };
	l5->platforms[10] = { 420, 320, 80, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 80, 70, 65 };
	l5->platforms[11] = { 640, 360, 80, 22, PLATFORM_SOLID, 0, 0, 0, 0, 0, 0, 80, 70, 65 };
	l5->platforms[12] = { 380, 160, 70, 20, PLATFORM_MOVING_V, 380, 160, 160, 320, 1.3f, 1, 0, 200, 220 };
	l5->platforms[13] = { 800, 200, 70, 20, PLATFORM_MOVING_V, 800, 200, 200, 380, 1.3f, 1, 0, 200, 220 };
	l5->switchCount = 7;
	l5->switches[0] = { 140, 182, 40, 10, 0, false, 0, false };
	l5->switches[1] = { 360, 222, 40, 10, 1, false, 1, false };
	l5->switches[2] = { 570, 262, 40, 10, 2, false, 2, false };
	l5->switches[3] = { 780, 302, 40, 10, 3, false, 3, false };
	l5->switches[4] = { 990, 362, 40, 10, 4, false, 4, false };
	l5->switches[5] = { 230, 302, 40, 10, 5, false, -1, true };
	l5->switches[6] = { 450, 342, 40, 10, 6, false, -1, true };
	l5->laserCount = 5;
	l5->lasers[0] = { 270, 100, 18, 120, 0, true, true, 0 };
	l5->lasers[1] = { 480, 100, 18, 160, 1, true, true, 0 };
	l5->lasers[2] = { 690, 100, 18, 200, 2, true, true, 0 };
	l5->lasers[3] = { 900, 100, 18, 240, 3, true, true, 0 };
	l5->lasers[4] = { 1020, 100, 18, 360, 4, true, true, 0 };
	l5->enemyCount = 1;
	l5->enemies[0] = { 550, 100, 30, 34, 300, 700, 1.2f, 4.6f, -1, 0, 140 };
	l5->birdCount = 2;
	l5->birds[0] = { 180, 600, 180, 600, 0, 0, 1.2f, 0.0f, 4.4f, 30, 22, 0, 150 };
	l5->birds[1] = { 1000, 600, 1000, 600, 0, 0, 1.2f, 1.1f, 4.4f, 30, 22, 0, 150 };
}

void loadLevel(int levelIdx) {
	if (levelIdx < 0 || levelIdx >= TOTAL_LEVELS) return;
	currentLevelIndex = levelIdx;
	LevelData* cur = &levels[currentLevelIndex];

	player.x = cur->spawnX; player.y = cur->spawnY;
	player.vx = 0; player.vy = 0;
	player.width = 28; player.height = 42;
	player.isGrounded = false; player.facingRight = true;
	player.animState = ANIM_IDLE; player.animTimer = 0; player.animFrame = 0;
	player.coyoteTimer = 0; player.jumpBufferTimer = 0; player.hitCooldown = 0;

	activeCloneCount = 0;
	for (int i = 0; i < MAX_CLONES; i++) {
		clones[i].isActive = false;
		clones[i].width = 28; clones[i].height = 42;
		clones[i].hasSeparated = false;
	}

	currentFrameTick = 0;
	timePenaltyFrames = 0;
	loopsUsedInLevel = 1;
	paradoxAnimationTimer = 0;
	rewindAnimationTimer = 0;
	screenShakeOffset = 0.0f;
	gameScore.currentScore = 0;

	for (int i = 0; i < cur->switchCount; i++) cur->switches[i].isPressed = false;
	for (int i = 0; i < cur->laserCount; i++) cur->lasers[i].isActive = true;

	for (int i = 0; i < cur->enemyCount; i++) {
		cur->enemies[i].x = cur->enemies[i].maxX;
		cur->enemies[i].direction = -1;
		cur->enemies[i].fireTimer = 0;
	}
	for (int i = 0; i < cur->birdCount; i++) {
		cur->birds[i].x = cur->birds[i].spawnX;
		cur->birds[i].y = cur->birds[i].spawnY;
		cur->birds[i].vx = 0; cur->birds[i].vy = 0;
		cur->birds[i].fireTimer = 0;
	}
	clearAllBullets();

	gameState = STATE_PLAYING;
	triggerSound("sounds/level_start.wav");
	playBackgroundMusic(LEVEL_MUSIC_FILE[currentLevelIndex]);
	menuMusicActive = false;
}

void resetLoop(bool isParadox) {
	LevelData* cur = &levels[currentLevelIndex];
	for (int i = 0; i < activeCloneCount; i++) clones[i].hasSeparated = false;

	if (isParadox) {
		gameState = STATE_PARADOX_RESET;
		paradoxAnimationTimer = 30;
		totalParadoxCount++;
		triggerSound("sounds/paradox_alert.wav");
		for (int i = 0; i < 35; i++) {
			float angle = (float)(rand() % 360) * (float)M_PI / 180.0f;
			float spd = (float)(rand() % 6 + 2);
			spawnParticle(player.x + 14, player.y + 21, cos(angle)*spd, sin(angle)*spd, 255, 30, 80, 5.0f, 0.7f);
		}
	}
	else {
		gameState = STATE_LOOP_REWIND;
		rewindAnimationTimer = 20;
		loopsUsedInLevel++;
		if (activeCloneCount < MAX_CLONES) {
			Clone* nc = &clones[activeCloneCount];
			for (int i = 0; i < currentFrameTick; i++) nc->history[i] = currentRecording[i];
			nc->totalRecordedFrames = currentFrameTick;
			nc->isActive = true;
			nc->hasSeparated = false;
			nc->width = 28; nc->height = 42;
			activeCloneCount++;
		}
		triggerSound("sounds/time_rewind.wav");
	}

	player.x = cur->spawnX; player.y = cur->spawnY;
	player.vx = 0; player.vy = 0; player.isGrounded = false; player.hitCooldown = 0;
	currentFrameTick = 0; timePenaltyFrames = 0;

	for (int i = 0; i < cur->birdCount; i++) {
		cur->birds[i].x = cur->birds[i].spawnX;
		cur->birds[i].y = cur->birds[i].spawnY;
		cur->birds[i].vx = 0; cur->birds[i].vy = 0;
		cur->birds[i].fireTimer = 0;
	}
	for (int i = 0; i < cur->enemyCount; i++) {
		cur->enemies[i].x = cur->enemies[i].maxX;
		cur->enemies[i].direction = -1;
		cur->enemies[i].fireTimer = 0;
	}
	clearAllBullets();
}

#endif