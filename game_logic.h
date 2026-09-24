#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "game_core.h"

void updateGame() {
	globalAnimTimer += 0.04f;
	updateParticles();
	updateBackgroundMusicLoop();

	// ESC Key
	bool escPressed = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
	if (escPressed && !prevEscKey) {
		if (gameState == STATE_PLAYING || gameState == STATE_LEVEL_SELECT ||
			gameState == STATE_INSTRUCTIONS || gameState == STATE_CREDITS ||
			gameState == STATE_CHARACTER_SELECT) {
			goToMenu();
		}
	}
	prevEscKey = escPressed;

	// Volume Control
	bool volUpPressed = (GetAsyncKeyState(VK_ADD) & 0x8000) != 0;
	if (volUpPressed && !prevVolUpKey) setMusicVolume(musicVolume + 100);
	prevVolUpKey = volUpPressed;

	bool volDownPressed = (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) != 0;
	if (volDownPressed && !prevVolDownKey) setMusicVolume(musicVolume - 100);
	prevVolDownKey = volDownPressed;

	// Paradox Animation
	if (gameState == STATE_PARADOX_RESET) {
		paradoxAnimationTimer--;
		screenShakeOffset = (float)(rand() % 14 - 7);
		if (paradoxAnimationTimer <= 0) {
			screenShakeOffset = 0.0f;
			gameState = STATE_PLAYING;
		}
		return;
	}

	// Rewind Animation
	if (gameState == STATE_LOOP_REWIND) {
		rewindAnimationTimer--;
		screenShakeOffset = (float)(rand() % 6 - 3);
		if (rewindAnimationTimer <= 0) {
			screenShakeOffset = 0.0f;
			gameState = STATE_PLAYING;
		}
		return;
	}

	if (gameState != STATE_PLAYING) return;

	LevelData* cur = &levels[currentLevelIndex];

	// Hit Cooldown
	if (player.hitCooldown > 0) player.hitCooldown--;

	// Time Limit
	if (currentFrameTick + timePenaltyFrames >= cur->loopDurationFrames) {
		resetLoop(false);
		return;
	}

	// Input
	bool isLeft = (GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000);
	bool isRight = (GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000);
	bool isJump = (GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000);
	bool isRewind = (GetAsyncKeyState('R') & 0x8000) != 0;

	if (isRewind && !prevRewindKey) {
		resetLoop(false);
		prevRewindKey = isRewind;
		return;
	}
	prevRewindKey = isRewind;

	if (isJump && !prevJumpKey) player.jumpBufferTimer = 8;
	else if (player.jumpBufferTimer > 0) player.jumpBufferTimer--;
	prevJumpKey = isJump;

	if (player.isGrounded) player.coyoteTimer = 7;
	else if (player.coyoteTimer > 0) player.coyoteTimer--;

	// Horizontal Movement
	float moveAcc = 1.1f;
	float maxSpeed = 5.2f;
	float friction = 0.80f;

	if (isLeft && !isRight) {
		player.vx -= moveAcc;
		if (player.vx < -maxSpeed) player.vx = -maxSpeed;
		player.facingRight = false;
	}
	else if (isRight && !isLeft) {
		player.vx += moveAcc;
		if (player.vx > maxSpeed) player.vx = maxSpeed;
		player.facingRight = true;
	}
	else {
		player.vx *= friction;
		if (fabs(player.vx) < 0.15f) player.vx = 0;
	}

	// Jump
	if (player.jumpBufferTimer > 0 && player.coyoteTimer > 0) {
		player.vy = 14.0f;
		player.isGrounded = false;
		player.coyoteTimer = 0;
		player.jumpBufferTimer = 0;
		triggerSound("sounds/jump.wav");
		for (int i = 0; i < 6; i++) {
			spawnParticle(player.x + 14, player.y, (rand() % 6 - 3) * 1.0f, (float)(rand() % 3 + 1), 180, 220, 255, 3.0f, 0.4f);
		}
	}

	// Gravity
	player.vy -= 0.72f;
	if (player.vy < -16.0f) player.vy = -16.0f;

	// Moving Platforms
	for (int i = 0; i < cur->platformCount; i++) {
		Platform* p = &cur->platforms[i];
		if (p->type == PLATFORM_MOVING_H) {
			p->x += p->speed * p->direction;
			if (p->x >= p->maxBound) { p->x = p->maxBound; p->direction = -1; }
			if (p->x <= p->minBound) { p->x = p->minBound; p->direction = 1; }
		}
		else if (p->type == PLATFORM_MOVING_V) {
			p->y += p->speed * p->direction;
			if (p->y >= p->maxBound) { p->y = p->maxBound; p->direction = -1; }
			if (p->y <= p->minBound) { p->y = p->minBound; p->direction = 1; }
		}
	}

	// Horizontal Collision
	player.x += player.vx;
	for (int i = 0; i < cur->platformCount; i++) {
		Platform* p = &cur->platforms[i];
		if (checkAABB(player.x, player.y, player.width, player.height, p->x, p->y, p->w, p->h)) {
			if (player.vx > 0) player.x = p->x - player.width;
			else if (player.vx < 0) player.x = p->x + p->w;
			player.vx = 0;
		}
	}

	// Vertical Collision
	player.y += player.vy;
	player.isGrounded = false;
	for (int i = 0; i < cur->platformCount; i++) {
		Platform* p = &cur->platforms[i];
		if (checkAABB(player.x, player.y, player.width, player.height, p->x, p->y, p->w, p->h)) {
			if (player.vy < 0) {
				player.y = p->y + p->h;
				player.vy = 0;
				player.isGrounded = true;
				if (p->type == PLATFORM_MOVING_H) {
					player.x += p->speed * p->direction;
				}
			}
			else if (player.vy > 0) {
				player.y = p->y - player.height;
				player.vy = 0;
			}
		}
	}

	// Screen Bounds
	if (player.x < 40) player.x = 40;
	if (player.x > SCREEN_WIDTH - 40 - player.width) player.x = SCREEN_WIDTH - 40 - player.width;
	if (player.y < 0) {
		resetLoop(true);
		return;
	}

	// ==================== HAZARDS (only after first loop) ====================
	if (loopsUsedInLevel >= 2) {
		// Enemies
		for (int i = 0; i < cur->enemyCount; i++) {
			Enemy* en = &cur->enemies[i];
			en->x += en->speed * en->direction;
			if (en->x >= en->maxX) { en->x = en->maxX; en->direction = -1; }
			if (en->x <= en->minX) { en->x = en->minX; en->direction = 1; }

			en->fireTimer++;
			if (en->fireTimer >= en->fireInterval) {
				en->fireTimer = 0;
				spawnBullet(en->x + en->w / 2, en->y + en->h / 2, en->bulletSpeed * en->direction, 0.0f);
			}
		}

		// Birds
		for (int i = 0; i < cur->birdCount; i++) {
			Bird* bird = &cur->birds[i];
			float targetX = player.x + player.width / 2 - bird->w / 2;
			float targetY = player.y + player.height / 2 - bird->h / 2;
			float dx = targetX - bird->x;
			float dy = targetY - bird->y;
			float dist = sqrtf(dx * dx + dy * dy);
			if (dist < 1.0f) dist = 1.0f;
			float dirX = dx / dist;
			float dirY = dy / dist;

			if (dist < 55.0f) {
				bird->vx = -dirX * 4.5f;
				bird->vy = -dirY * 4.5f;
			}
			else {
				bird->wobblePhase += 0.12f;
				float wobble = sinf(bird->wobblePhase) * 0.6f;
				float perpX = -dirY;
				float perpY = dirX;
				bird->vx = dirX * bird->speed + perpX * wobble;
				bird->vy = dirY * bird->speed + perpY * wobble;
			}

			bird->x += bird->vx;
			bird->y += bird->vy;

			if (bird->y < 100) bird->y = 100;
			if (bird->y > SCREEN_HEIGHT - 40) bird->y = SCREEN_HEIGHT - 40;
			if (bird->x < 50) bird->x = 50;
			if (bird->x > SCREEN_WIDTH - 50) bird->x = SCREEN_WIDTH - 50;

			bird->fireTimer++;
			if (bird->fireTimer >= bird->fireInterval) {
				bird->fireTimer = 0;
				spawnBullet(bird->x + bird->w / 2, bird->y + bird->h / 2, dirX * bird->bulletSpeed, dirY * bird->bulletSpeed);
			}
		}

		// Bullets
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (!bullets[i].active) continue;
			bullets[i].x += bullets[i].vx;
			bullets[i].y += bullets[i].vy;

			if (bullets[i].x < 0 || bullets[i].x > SCREEN_WIDTH || bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT) {
				bullets[i].active = false;
				continue;
			}

			if (checkAABB(player.x, player.y, player.width, player.height, bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h)) {
				bullets[i].active = false;
				if (player.hitCooldown <= 0) {
					timePenaltyFrames += 2 * FPS;
					player.hitCooldown = 25;
					triggerSound("sounds/hit_fire.wav");
					for (int p = 0; p < 8; p++) {
						spawnParticle(player.x + 14, player.y + 21, (rand() % 6 - 3)*1.2f, (rand() % 6 - 3)*1.2f, 255, 180, 40, 3.5f, 0.45f);
					}
				}
			}
		}

		// Enemy Body
		for (int i = 0; i < cur->enemyCount; i++) {
			Enemy* en = &cur->enemies[i];
			if (checkAABB(player.x, player.y, player.width, player.height, en->x, en->y, en->w, en->h)) {
				if (player.hitCooldown <= 0) {
					timePenaltyFrames += 1 * FPS;
					player.hitCooldown = 30;
					triggerSound("sounds/hit_fire.wav");
				}
			}
		}

		// Bird Body
		for (int i = 0; i < cur->birdCount; i++) {
			Bird* bird = &cur->birds[i];
			if (checkAABB(player.x, player.y, player.width, player.height, bird->x, bird->y, bird->w, bird->h)) {
				if (player.hitCooldown <= 0) {
					timePenaltyFrames += 1 * FPS;
					player.hitCooldown = 30;
					triggerSound("sounds/hit_fire.wav");
				}
			}
		}
	}

	// Animation State
	if (!player.isGrounded) {
		player.animState = (player.vy > 0) ? ANIM_JUMP : ANIM_FALL;
	}
	else if (fabs(player.vx) > 0.5f) {
		player.animState = ANIM_RUN;
		player.animTimer += 0.2f;
		if (player.animTimer >= 1.0f) {
			player.animTimer = 0;
			player.animFrame = (player.animFrame + 1) % 4;
		}
	}
	else {
		player.animState = ANIM_IDLE;
		player.animFrame = 0;
	}

	// Record Frame
	currentRecording[currentFrameTick].x = player.x;
	currentRecording[currentFrameTick].y = player.y;
	currentRecording[currentFrameTick].vx = player.vx;
	currentRecording[currentFrameTick].vy = player.vy;
	currentRecording[currentFrameTick].animState = player.animState;
	currentRecording[currentFrameTick].facingRight = player.facingRight;
	currentRecording[currentFrameTick].isGrounded = player.isGrounded;
	currentRecording[currentFrameTick].animFrame = player.animFrame;

	// Clones
	for (int c = 0; c < activeCloneCount; c++) {
		Clone* cl = &clones[c];
		if (!cl->isActive) continue;

		if (currentFrameTick < cl->totalRecordedFrames) {
			FrameSnapshot snap = cl->history[currentFrameTick];
			cl->x = snap.x;
			cl->y = snap.y;
			cl->vx = snap.vx;
			cl->vy = snap.vy;
			cl->animState = snap.animState;
			cl->animFrame = snap.animFrame;
			cl->facingRight = snap.facingRight;

			if ((currentFrameTick + c * 2) % 5 == 0) {
				spawnParticle(cl->x + 14, cl->y + 21, (rand() % 4 - 2)*0.3f, (rand() % 4 - 2)*0.3f, 0, 220, 255, 3.0f, 0.4f);
			}

			bool overlapping = checkAABB(player.x, player.y, player.width, player.height, cl->x, cl->y, cl->width, cl->height);
			if (!overlapping) cl->hasSeparated = true;

			if (cl->hasSeparated && currentFrameTick > 40 && overlapping) {
				if (player.hitCooldown <= 0) {
					timePenaltyFrames += 5 * FPS;
					player.hitCooldown = 45;
					triggerParadoxWarning();
				}
			}
		}
		else {
			cl->animState = ANIM_IDLE;
		}
	}

	// ==================== SWITCHES ====================
	for (int i = 0; i < cur->switchCount; i++) {
		Switch* sw = &cur->switches[i];
		bool playerOnSwitch = checkAABB(player.x, player.y, player.width, player.height, sw->x, sw->y, sw->w, sw->h);
		bool cloneOnSwitch = false;

		for (int c = 0; c < activeCloneCount; c++) {
			Clone* cl = &clones[c];
			if (cl->isActive && currentFrameTick < cl->totalRecordedFrames &&
				checkAABB(cl->x, cl->y, cl->width, cl->height, sw->x, sw->y, sw->w, sw->h)) {
				cloneOnSwitch = true;
				break;
			}
		}

		bool currentlyStepped = (playerOnSwitch || cloneOnSwitch);

		if (currentlyStepped && !sw->isPressed) {
			if (sw->isFake) {
				if (player.hitCooldown <= 0) {
					timePenaltyFrames += 1 * FPS;
					player.hitCooldown = 25;
					triggerSound("sounds/paradox_alert.wav");
				}
			}
			else {
				triggerSound("sounds/switch.wav");
				for (int p = 0; p < 8; p++) {
					spawnParticle(sw->x + sw->w / 2, sw->y + sw->h, (rand() % 6 - 3)*1.0f, (float)(rand() % 4 + 1), 0, 255, 180, 3.0f, 0.4f);
				}
			}
		}
		sw->isPressed = currentlyStepped;
	}

	// ==================== LASERS ====================
	for (int i = 0; i < cur->laserCount; i++) {
		LaserGate* laser = &cur->lasers[i];
		laser->pulsePhase += 0.15f;

		bool deactivated = false;
		for (int s = 0; s < cur->switchCount; s++) {
			if (cur->switches[s].targetLaserId == laser->laserId && cur->switches[s].isPressed) {
				deactivated = true;
				break;
			}
		}
		laser->isActive = !deactivated;

		if (laser->isActive) {
			if (checkAABB(player.x, player.y, player.width, player.height, laser->x, laser->y, laser->w, laser->h)) {
				triggerSound("sounds/laser_hit.wav");
				resetLoop(true);
				return;
			}
		}
	}

	// ==================== EXIT PORTAL ====================
	ExitPortal* portal = &cur->portal;
	portal->rotationAngle += 3.0f;
	portal->pulse = (float)sin(globalAnimTimer * 3.0f) * 4.0f;

	float distX = (player.x + player.width / 2) - portal->x;
	float distY = (player.y + player.height / 2) - portal->y;
	float dist = sqrt(distX*distX + distY*distY);

	if (dist < portal->radius + 15.0f && portal->isUnlocked) {
		triggerSound("sounds/portal_win.wav");

		// Score System
		updateScore();
		gameScore.levelsCleared++;
		saveGameData();

		if (currentLevelIndex + 1 < TOTAL_LEVELS) {
			levelUnlocked[currentLevelIndex + 1] = true;
			gameState = STATE_LEVEL_CLEAR;
		}
		else {
			gameState = STATE_GAME_VICTORY;
		}
		return;
	}

	currentFrameTick++;
}

#endif