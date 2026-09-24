#ifndef GAME_RENDER_H
#define GAME_RENDER_H

#include "game_core.h"

void drawPlayerCharacter(float x, float y, bool facingRight, int animState, int frame) {
	int imgID = -1;
	int c = selectedCharacter;
	if (c < 0 || c >= TOTAL_CHARACTERS) c = 0;

	if (animState == ANIM_IDLE)
		imgID = facingRight ? characterIdleRight[c][frame % 6] : characterIdleLeft[c][frame % 6];
	else if (animState == ANIM_RUN)
		imgID = facingRight ? characterRunRight[c][frame % 8] : characterRunLeft[c][frame % 8];
	else if (animState == ANIM_JUMP)
		imgID = characterJumpUp[c][frame % 6];
	else if (animState == ANIM_FALL)
		imgID = characterFall[c][frame % 6];

	if (imgID != -1)
		iShowImage(x + screenShakeOffset - 10, y, 48, 58, imgID);
}

void drawCloneCharacter(float x, float y, bool facingRight, int animState, int frame, int cloneNumber) {
	float cx = x + 14.0f;
	float cy = y + 29.0f;
	iSetColor(0, 120, 255); iCircle(cx, cy, 30, 20);
	iSetColor(0, 200, 255); iCircle(cx, cy, 25, 20);
	iSetColor(80, 240, 255); iCircle(cx, cy, 20, 20);
	drawPlayerCharacter(x, y, facingRight, animState, frame);
	iSetColor(0, 230, 255);
	iLine(x - 2, y + 8, x + 4, y + 4);
	iLine(x + 34, y + 8, x + 40, y + 4);
	iLine(x - 1, y + 48, x + 5, y + 52);
	iLine(x + 33, y + 48, x + 39, y + 52);
}

void drawExitPortal(ExitPortal* portal) {
	float px = portal->x + screenShakeOffset;
	float py = portal->y;
	float r = portal->radius + portal->pulse;

	for (int i = 0; i < 12; i++) {
		float angle = portal->rotationAngle + (i * 30.0f) * (float)M_PI / 180.0f;
		iSetColor(255, 200, 50);
		iFilledCircle(px + cos(angle) * r, py + sin(angle) * r, 4, 6);
	}
	iSetColor(60, 20, 100);  iFilledCircle(px, py, r * 0.75f, 20);
	iSetColor(180, 50, 255); iFilledCircle(px, py, r * 0.5f, 16);
	iSetColor(255, 240, 150); iFilledCircle(px, py, r * 0.25f, 12);
	iSetColor(255, 255, 255);
	iText(px - 35, py - 50, "EXIT PORTAL", GLUT_BITMAP_HELVETICA_12);
}

void drawLaserGate(LaserGate* laser) {
	if (!laser->isActive) {
		iSetColor(50, 50, 60);
		iFilledRectangle(laser->x, laser->y, laser->w, laser->h);
		return;
	}
	int pulseIntensity = (int)(sin(laser->pulsePhase) * 40.0f + 215.0f);
	iSetColor(pulseIntensity, 20, 60);
	iFilledRectangle(laser->x - 2, laser->y, laser->w + 4, laser->h);
	iSetColor(255, 220, 230);
	iFilledRectangle(laser->x + 6, laser->y, laser->w - 12, laser->h);
	iSetColor(90, 100, 120);
	iFilledRectangle(laser->x - 4, laser->y, laser->w + 8, 8);
	iFilledRectangle(laser->x - 4, laser->y + laser->h - 8, laser->w + 8, 8);
}

void drawSwitch(Switch* sw) {
	iSetColor(40, 50, 65);
	iFilledRectangle(sw->x - 4, sw->y, sw->w + 8, sw->h + 4);
	if (sw->isPressed) {
		iSetColor(0, 255, 180);
		iFilledRectangle(sw->x, sw->y, sw->w, 4);
		iSetColor(0, 255, 220);
		iText(sw->x - 10, sw->y - 14, "ACTIVE", GLUT_BITMAP_HELVETICA_10);
	}
	else {
		iSetColor(255, 120, 30);
		iFilledRectangle(sw->x, sw->y, sw->w, sw->h);
		iSetColor(200, 200, 200);
		iText(sw->x - 12, sw->y - 14, "SWITCH", GLUT_BITMAP_HELVETICA_10);
	}
}

void drawEnemy(Enemy* en) {
	const float drawW = 52.0f;
	const float drawH = 62.0f;
	bool isShooting = (en->fireTimer < 12);
	int frame = ((int)(globalAnimTimer * 12.0f)) % 8;
	int imgID = -1;

	if (isShooting)
		imgID = (en->direction > 0) ? enemyShootRight[frame] : enemyShootLeft[frame];
	else
		imgID = (en->direction > 0) ? enemyWalkRight[frame] : enemyWalkLeft[frame];

	if (imgID != -1) {
		float drawX = en->x + screenShakeOffset + (en->w - drawW) / 2.0f;
		float drawY = en->y + en->h - drawH;
		iShowImage(drawX, drawY, drawW, drawH, imgID);
	}
	else {
		iSetColor(80, 20, 20);
		iFilledRectangle(en->x + screenShakeOffset, en->y, en->w, en->h);
	}
}

void drawBullet(Bullet* b) {
	iSetColor(255, 200, 60);
	iFilledRectangle(b->x + screenShakeOffset, b->y, b->w, b->h);
	iSetColor(255, 255, 180);
	iFilledCircle(b->x + b->w / 2 + screenShakeOffset, b->y + b->h / 2, 3, 8);
}

void drawBird(Bird* bird) {
	int frame = ((int)(globalAnimTimer * 12.0f)) % 6;
	if (birdSprite[frame] != -1) {
		float drawW = 72.0f;
		float drawH = 48.0f;
		float drawX = bird->x + screenShakeOffset - (drawW - bird->w) / 2.0f;
		float drawY = bird->y - (drawH - bird->h) / 2.0f;
		iShowImage(drawX, drawY, drawW, drawH, birdSprite[frame]);
	}
}

void drawHUD() {
	LevelData* cur = &levels[currentLevelIndex];
	iSetColor(15, 22, 35);
	iFilledRectangle(0, 630, SCREEN_WIDTH, 70);
	iSetColor(40, 60, 90);
	iLine(0, 630, SCREEN_WIDTH, 630);

	float timeRemaining = (cur->loopDurationFrames - currentFrameTick - timePenaltyFrames) / (float)FPS;
	if (timeRemaining < 0) timeRemaining = 0;

	char timerStr[64];
	sprintf(timerStr, "TIME REMAINING: %05.2fs", timeRemaining);
	if (timeRemaining <= 5.0f) iSetColor(255, 50, 60);
	else iSetColor(0, 230, 255);
	iText(40, 665, timerStr, GLUT_BITMAP_TIMES_ROMAN_24);

	float levelDurationSec = cur->loopDurationFrames / (float)FPS;
	float barWidth = 300.0f;
	float currentBarWidth = (timeRemaining / levelDurationSec) * barWidth;
	if (currentBarWidth < 0) currentBarWidth = 0;
	if (currentBarWidth > barWidth) currentBarWidth = barWidth;

	iSetColor(30, 40, 60);
	iFilledRectangle(40, 642, barWidth, 14);
	if (timeRemaining <= 5.0f) iSetColor(255, 60, 70);
	else iSetColor(0, 255, 180);
	iFilledRectangle(40, 642, currentBarWidth, 14);
	iSetColor(255, 255, 255);
	iRectangle(40, 642, barWidth, 14);

	iSetColor(255, 255, 255);
	iText(400, 668, cur->name, GLUT_BITMAP_HELVETICA_18);

	char loopStr[64];
	sprintf(loopStr, "TEMPORAL LOOP: #%d", loopsUsedInLevel);
	iSetColor(255, 200, 50);
	iText(400, 644, loopStr, GLUT_BITMAP_HELVETICA_12);

	char scoreText[80];
	sprintf(scoreText, "Score: %d   High: %d", gameScore.currentScore, gameScore.highestScore);
	iSetColor(255, 255, 100);
	iText(400, 620, scoreText, GLUT_BITMAP_HELVETICA_12);

	if (activeCloneCount > 0) {
		iSetColor(0, 255, 255);
		iFilledCircle(700, 656, 6, 8);
		char cloneStatusStr[48];
		sprintf(cloneStatusStr, "%d CLONE(S) ACTIVE", activeCloneCount);
		iText(715, 651, cloneStatusStr, GLUT_BITMAP_HELVETICA_12);
	}

	iSetColor(180, 190, 210);
	iText(920, 668, "[ARROWS / WASD] Move & Jump", GLUT_BITMAP_HELVETICA_12);
	iText(920, 644, "[R] Rewind | [ESC] Menu", GLUT_BITMAP_HELVETICA_12);
}

void iDraw() {
	iClear();

	// ========== MENU ==========
	if (gameState == STATE_MENU) {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBackgroundID);

		float btnW = 260, btnH = 45, startX = 40;

		iSetColor(0, 160, 110); iFilledRectangle(startX, 415, btnW, btnH);
		iSetColor(255, 255, 255); iRectangle(startX, 415, btnW, btnH);
		iText(startX + 65, 430, "START GAME", GLUT_BITMAP_HELVETICA_18);

		iSetColor(30, 70, 120); iFilledRectangle(startX, 355, btnW, btnH);
		iSetColor(255, 255, 255); iRectangle(startX, 355, btnW, btnH);
		iText(startX + 55, 370, "LEVEL SELECT", GLUT_BITMAP_HELVETICA_18);

		iSetColor(30, 70, 120); iFilledRectangle(startX, 295, btnW, btnH);
		iSetColor(255, 255, 255); iRectangle(startX, 295, btnW, btnH);
		iText(startX + 45, 310, "SELECT CHARACTER", GLUT_BITMAP_HELVETICA_18);

		iSetColor(30, 70, 120); iFilledRectangle(startX, 235, btnW, btnH);
		iSetColor(255, 255, 255); iRectangle(startX, 235, btnW, btnH);
		iText(startX + 58, 250, "HOW TO PLAY", GLUT_BITMAP_HELVETICA_18);

		iSetColor(30, 70, 120); iFilledRectangle(startX, 175, btnW, btnH);
		iSetColor(255, 255, 255); iRectangle(startX, 175, btnW, btnH);
		iText(startX + 80, 190, "CREDITS", GLUT_BITMAP_HELVETICA_18);

		iSetColor(120, 40, 40); iFilledRectangle(startX, 115, btnW, btnH);
		iSetColor(255, 255, 255); iRectangle(startX, 115, btnW, btnH);
		iText(startX + 100, 130, "EXIT", GLUT_BITMAP_HELVETICA_18);
		return;
	}
	// ========== INSTRUCTIONS (HOW TO PLAY) ==========
	if (gameState == STATE_INSTRUCTIONS) {
		if (instructionsBackgroundID != -1)
			iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, instructionsBackgroundID);
		else {
			iSetColor(10, 15, 25);
			iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}

		// Dark panel behind text for better visibility
		iSetColor(0, 0, 0);
		iFilledRectangle(140, 140, 520, 480);

		// Title
		iSetColor(0, 255, 220);
		iText(SCREEN_WIDTH / 2 - 100, 620, "HOW TO PLAY", GLUT_BITMAP_TIMES_ROMAN_24);

		// OBJECTIVE
		iSetColor(255, 220, 50);
		iText(160, 540, "OBJECTIVE:", GLUT_BITMAP_HELVETICA_18);

		iSetColor(255, 255, 255);
		iText(180, 510, "- Reach the EXIT PORTAL before time runs out", GLUT_BITMAP_HELVETICA_12);
		iText(180, 485, "- Use switches to deactivate laser gates", GLUT_BITMAP_HELVETICA_12);
		iText(180, 460, "- Press R to Rewind time and create a Clone of yourself", GLUT_BITMAP_HELVETICA_12);

		// CONTROLS
		iSetColor(255, 220, 50);
		iText(160, 410, "CONTROLS:", GLUT_BITMAP_HELVETICA_18);

		iSetColor(255, 255, 255);
		iText(180, 380, "- ARROWS / WASD  :  Move", GLUT_BITMAP_HELVETICA_12);
		iText(180, 355, "- SPACE / W / UP :  Jump", GLUT_BITMAP_HELVETICA_12);
		iText(180, 330, "- R              :  Rewind (create clone)", GLUT_BITMAP_HELVETICA_12);
		iText(180, 305, "- ESC            :  Return to Menu", GLUT_BITMAP_HELVETICA_12);

		// IMPORTANT
		iSetColor(255, 220, 50);
		iText(160, 255, "IMPORTANT:", GLUT_BITMAP_HELVETICA_18);

		iSetColor(255, 255, 255);
		iText(180, 225, "- After first loop, enemies and birds will appear and shoot", GLUT_BITMAP_HELVETICA_12);
		iText(180, 200, "- Touching your own clone after separation causes PARADOX", GLUT_BITMAP_HELVETICA_12);
		iText(180, 175, "- Fake switches will punish you with time penalty", GLUT_BITMAP_HELVETICA_12);

		// Back button
		iSetColor(80, 40, 40);
		iFilledRectangle(50, 50, 140, 40);
		iSetColor(255, 255, 255);
		iRectangle(50, 50, 140, 40);
		iText(80, 62, "<- BACK", GLUT_BITMAP_HELVETICA_18);
		return;
	}

	// ========== CREDITS ==========
	if (gameState == STATE_CREDITS) {
		if (creditsBackgroundID != -1)
			iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, creditsBackgroundID);
		else {
			iSetColor(10, 15, 25);
			iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}

		iSetColor(0, 255, 220);
		iText(SCREEN_WIDTH / 2 - 70, 580, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(255, 255, 255);
		iText(SCREEN_WIDTH / 2 - 140, 500, "LoopBound: Paradox", GLUT_BITMAP_HELVETICA_18);
		iText(SCREEN_WIDTH / 2 - 160, 465, "A Sci-Fi Time Loop Platformer", GLUT_BITMAP_HELVETICA_12);

		iSetColor(200, 220, 255);
		iText(SCREEN_WIDTH / 2 - 90, 400, "Developed by:", GLUT_BITMAP_HELVETICA_18);

		iSetColor(255, 255, 100);
		iText(SCREEN_WIDTH / 2 - 130, 360, "MAHSIN SIDDIQUE SABAB", GLUT_BITMAP_HELVETICA_18);
		iText(SCREEN_WIDTH / 2 - 110, 325, "ABU TALEB SHEIKH", GLUT_BITMAP_HELVETICA_18);
		iText(SCREEN_WIDTH / 2 - 100, 290, "TASRIF RASHID", GLUT_BITMAP_HELVETICA_18);

		iSetColor(180, 190, 210);
		iText(SCREEN_WIDTH / 2 - 130, 220, "Made with iGraphics + C++", GLUT_BITMAP_HELVETICA_12);

		iSetColor(80, 40, 40);
		iFilledRectangle(50, 50, 140, 40);
		iSetColor(255, 255, 255);
		iRectangle(50, 50, 140, 40);
		iText(80, 62, "<- BACK", GLUT_BITMAP_HELVETICA_18);
		return;
	}

	// ========== LEVEL SELECT ==========
	if (gameState == STATE_LEVEL_SELECT) {
		if (levelSelectBackgroundID != -1)
			iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, levelSelectBackgroundID);
		else {
			iSetColor(10, 15, 25);
			iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}

		iSetColor(0, 255, 220);
		iText(SCREEN_WIDTH / 2 - 120, 600, "SELECT LEVEL", GLUT_BITMAP_TIMES_ROMAN_24);

		for (int i = 0; i < TOTAL_LEVELS; i++) {
			float lx = 200 + (i % 3) * 300;
			float ly = 420 - (i / 3) * 160;

			if (levelUnlocked[i]) {
				iSetColor(25, 55, 90); iFilledRectangle(lx, ly, 240, 110);
				iSetColor(0, 230, 255); iRectangle(lx, ly, 240, 110);
				char lNum[32]; sprintf(lNum, "LEVEL %d", i + 1);
				iSetColor(255, 255, 255); iText(lx + 75, ly + 70, lNum, GLUT_BITMAP_HELVETICA_18);
			}
			else {
				iSetColor(25, 30, 40); iFilledRectangle(lx, ly, 240, 110);
				iSetColor(60, 70, 80); iRectangle(lx, ly, 240, 110);
				iSetColor(120, 130, 140); iText(lx + 80, ly + 55, "[LOCKED]", GLUT_BITMAP_HELVETICA_18);
			}
		}

		iSetColor(80, 40, 40); iFilledRectangle(50, 50, 140, 40);
		iSetColor(255, 255, 255); iRectangle(50, 50, 140, 40);
		iText(80, 62, "<- BACK", GLUT_BITMAP_HELVETICA_18);
		return;
	}

	// ========== CHARACTER SELECT ==========
	if (gameState == STATE_CHARACTER_SELECT) {
		if (menuBackgroundID != -1)
			iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBackgroundID);
		else {
			iSetColor(10, 15, 25);
			iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}

		iSetColor(0, 255, 220);
		iText(SCREEN_WIDTH / 2 - 150, 600, "SELECT CHARACTER", GLUT_BITMAP_TIMES_ROMAN_24);

		// GOAT
		iSetColor(selectedCharacter == 0 ? 0 : 30, selectedCharacter == 0 ? 180 : 70, selectedCharacter == 0 ? 140 : 120);
		iFilledRectangle(300, 300, 240, 160);
		iSetColor(255, 255, 255); iRectangle(300, 300, 240, 160);
		if (characterIdleRight[0][0] != -1) iShowImage(396, 345, 48, 58, characterIdleRight[0][0]);
		iText(390, 320, "GOAT", GLUT_BITMAP_HELVETICA_18);

		// HERO
		iSetColor(selectedCharacter == 1 ? 0 : 30, selectedCharacter == 1 ? 180 : 70, selectedCharacter == 1 ? 140 : 120);
		iFilledRectangle(660, 300, 240, 160);
		iSetColor(255, 255, 255); iRectangle(660, 300, 240, 160);
		if (characterIdleRight[1][0] != -1) iShowImage(756, 345, 48, 58, characterIdleRight[1][0]);
		iText(750, 320, "HERO", GLUT_BITMAP_HELVETICA_18);

		iSetColor(80, 40, 40); iFilledRectangle(50, 50, 140, 40);
		iSetColor(255, 255, 255); iRectangle(50, 50, 140, 40);
		iText(80, 62, "<- BACK", GLUT_BITMAP_HELVETICA_18);
		return;
	}

	// ========== LEVEL CLEAR ==========
	if (gameState == STATE_LEVEL_CLEAR) {
		iSetColor(10, 20, 35);
		iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		iSetColor(0, 255, 180);
		iText(SCREEN_WIDTH / 2 - 180, 500, "CHRONO ANOMALY RESOLVED!", GLUT_BITMAP_TIMES_ROMAN_24);

		char statStr[128];
		sprintf(statStr, "Level Cleared in %d Temporal Loops", loopsUsedInLevel);
		iSetColor(255, 255, 255);
		iText(SCREEN_WIDTH / 2 - 150, 440, statStr, GLUT_BITMAP_HELVETICA_18);

		char scoreStr[100];
		sprintf(scoreStr, "Score: %d    High Score: %d", gameScore.currentScore, gameScore.highestScore);
		iSetColor(255, 255, 100);
		iText(SCREEN_WIDTH / 2 - 140, 400, scoreStr, GLUT_BITMAP_HELVETICA_18);

		iSetColor(0, 160, 110);
		iFilledRectangle(SCREEN_WIDTH / 2 - 110, 280, 220, 45);
		iSetColor(255, 255, 255);
		iRectangle(SCREEN_WIDTH / 2 - 110, 280, 220, 45);
		iText(SCREEN_WIDTH / 2 - 65, 295, "NEXT LEVEL", GLUT_BITMAP_HELVETICA_18);

		iSetColor(50, 60, 80);
		iFilledRectangle(SCREEN_WIDTH / 2 - 110, 210, 220, 45);
		iSetColor(255, 255, 255);
		iRectangle(SCREEN_WIDTH / 2 - 110, 210, 220, 45);
		iText(SCREEN_WIDTH / 2 - 50, 225, "MAIN MENU", GLUT_BITMAP_HELVETICA_18);
		return;
	}

	// ========== VICTORY ==========
	if (gameState == STATE_GAME_VICTORY) {
		iSetColor(10, 20, 35);
		iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		iSetColor(255, 215, 0);
		iText(SCREEN_WIDTH / 2 - 200, 500, "TIMELINE STABILIZED!", GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(255, 255, 255);
		iText(SCREEN_WIDTH / 2 - 230, 420, "Congratulations! You escaped the facility!", GLUT_BITMAP_HELVETICA_18);

		char scoreStr[100];
		sprintf(scoreStr, "Final High Score: %d", gameScore.highestScore);
		iSetColor(255, 255, 100);
		iText(SCREEN_WIDTH / 2 - 110, 370, scoreStr, GLUT_BITMAP_HELVETICA_18);

		iSetColor(0, 160, 110);
		iFilledRectangle(SCREEN_WIDTH / 2 - 110, 250, 220, 45);
		iSetColor(255, 255, 255);
		iRectangle(SCREEN_WIDTH / 2 - 110, 250, 220, 45);
		iText(SCREEN_WIDTH / 2 - 60, 265, "MAIN MENU", GLUT_BITMAP_HELVETICA_18);
		return;
	}

	// ========== PLAYING ==========
	LevelData* cur = &levels[currentLevelIndex];

	if (levelBackgroundID[currentLevelIndex] != -1)
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, levelBackgroundID[currentLevelIndex]);
	else {
		iSetColor(14, 18, 28);
		iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	bool hasBgArt = (levelBackgroundID[currentLevelIndex] != -1);

	// Platforms
	for (int i = 0; i < cur->platformCount; i++) {
		Platform* p = &cur->platforms[i];
		bool isBoundary = (i == 0 || i == 1 || i == 2);
		if (hasBgArt && isBoundary) continue;

		if (p->type == PLATFORM_SOLID && p->h <= 30) {
			if (platformGrassImg != -1)
				iShowImage(p->x + screenShakeOffset, p->y - 5, p->w, p->h + 12, platformGrassImg);
			else {
				iSetColor(p->r, p->g, p->b);
				iFilledRectangle(p->x + screenShakeOffset, p->y, p->w, p->h);
			}
		}
		else {
			iSetColor(p->r, p->g, p->b);
			iFilledRectangle(p->x + screenShakeOffset, p->y, p->w, p->h);
			if (p->type == PLATFORM_MOVING_H || p->type == PLATFORM_MOVING_V)
				iSetColor(0, 255, 255);
			else
				iSetColor(0, 180, 255);
			iFilledRectangle(p->x + screenShakeOffset, p->y + p->h - 3, p->w, 3);
		}
	}

	for (int i = 0; i < cur->switchCount; i++) drawSwitch(&cur->switches[i]);
	for (int i = 0; i < cur->laserCount; i++) drawLaserGate(&cur->lasers[i]);
	drawExitPortal(&cur->portal);

	if (loopsUsedInLevel >= 2) {
		for (int i = 0; i < cur->enemyCount; i++) drawEnemy(&cur->enemies[i]);
		for (int i = 0; i < MAX_BULLETS; i++)
		if (bullets[i].active) drawBullet(&bullets[i]);
		for (int i = 0; i < cur->birdCount; i++) drawBird(&cur->birds[i]);
	}

	for (int c = 0; c < activeCloneCount; c++) {
		Clone* cl = &clones[c];
		if (cl->isActive && currentFrameTick < cl->totalRecordedFrames)
			drawCloneCharacter(cl->x + screenShakeOffset, cl->y, cl->facingRight, cl->animState, cl->animFrame, c + 1);
	}

	drawPlayerCharacter(player.x + screenShakeOffset, player.y, player.facingRight, player.animState, player.animFrame);
	drawParticles();
	drawHUD();

	// Effects
	if (gameState == STATE_PARADOX_RESET) {
		iSetColor(255, 0, 50);
		for (int i = 0; i < SCREEN_HEIGHT; i += 8)
		if ((i + paradoxAnimationTimer * 4) % 16 == 0)
			iFilledRectangle(0, i, SCREEN_WIDTH, 4);
		iSetColor(255, 255, 255);
		iText(SCREEN_WIDTH / 2 - 220, SCREEN_HEIGHT / 2, "PARADOX COLLAPSE DETECTED!", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (gameState == STATE_LOOP_REWIND) {
		iSetColor(0, 220, 255);
		for (int i = 0; i < SCREEN_WIDTH; i += 12)
		if ((i + rewindAnimationTimer * 6) % 24 == 0)
			iFilledRectangle(i, 0, 4, SCREEN_HEIGHT);
		iSetColor(255, 255, 255);
		iText(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2, "TEMPORAL LOOP REWOUND", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

#endif