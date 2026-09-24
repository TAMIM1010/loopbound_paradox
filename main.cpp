#include "game_core.h"
#include "game_logic.h"
#include "game_render.h"

void loadAllCharacterSprites() {
	char path[256];
	char legacyPath[256];
	for (int c = 0; c < TOTAL_CHARACTERS; c++) {
		for (int i = 0; i < 6; i++) {
			sprintf(path, "images/player/%s/idle_right_%d.png", CHARACTER_FOLDERS[c], i + 1);
			characterIdleRight[c][i] = iLoadImage(path);
			if (characterIdleRight[c][i] == -1 && c == 0) {
				sprintf(legacyPath, "images/player/idle_right_%d.png", i + 1);
				characterIdleRight[c][i] = iLoadImage(legacyPath);
			}
			if (characterIdleRight[c][i] == -1) characterIdleRight[c][i] = characterIdleRight[0][i];
		}
		for (int i = 0; i < 6; i++) {
			sprintf(path, "images/player/%s/idle_left_%d.png", CHARACTER_FOLDERS[c], i + 1);
			characterIdleLeft[c][i] = iLoadImage(path);
			if (characterIdleLeft[c][i] == -1 && c == 0) {
				sprintf(legacyPath, "images/player/idle_left_%d.png", i + 1);
				characterIdleLeft[c][i] = iLoadImage(legacyPath);
			}
			if (characterIdleLeft[c][i] == -1) characterIdleLeft[c][i] = characterIdleLeft[0][i];
		}
		for (int i = 0; i < 8; i++) {
			sprintf(path, "images/player/%s/run_right_%d.png", CHARACTER_FOLDERS[c], i + 1);
			characterRunRight[c][i] = iLoadImage(path);
			if (characterRunRight[c][i] == -1 && c == 0) {
				sprintf(legacyPath, "images/player/run_right_%d.png", i + 1);
				characterRunRight[c][i] = iLoadImage(legacyPath);
			}
			if (characterRunRight[c][i] == -1) characterRunRight[c][i] = characterRunRight[0][i];
		}
		for (int i = 0; i < 8; i++) {
			sprintf(path, "images/player/%s/run_left_%d.png", CHARACTER_FOLDERS[c], i + 1);
			characterRunLeft[c][i] = iLoadImage(path);
			if (characterRunLeft[c][i] == -1 && c == 0) {
				sprintf(legacyPath, "images/player/run_left_%d.png", i + 1);
				characterRunLeft[c][i] = iLoadImage(legacyPath);
			}
			if (characterRunLeft[c][i] == -1) characterRunLeft[c][i] = characterRunLeft[0][i];
		}
		for (int i = 0; i < 6; i++) {
			sprintf(path, "images/player/%s/jump_up_%d.png", CHARACTER_FOLDERS[c], i + 1);
			characterJumpUp[c][i] = iLoadImage(path);
			if (characterJumpUp[c][i] == -1 && c == 0) {
				sprintf(legacyPath, "images/player/jump_up_%d.png", i + 1);
				characterJumpUp[c][i] = iLoadImage(legacyPath);
			}
			if (characterJumpUp[c][i] == -1) characterJumpUp[c][i] = characterJumpUp[0][i];
		}
		for (int i = 0; i < 6; i++) {
			sprintf(path, "images/player/%s/fall_%d.png", CHARACTER_FOLDERS[c], i + 1);
			characterFall[c][i] = iLoadImage(path);
			if (characterFall[c][i] == -1 && c == 0) {
				sprintf(legacyPath, "images/player/fall_%d.png", i + 1);
				characterFall[c][i] = iLoadImage(legacyPath);
			}
			if (characterFall[c][i] == -1) characterFall[c][i] = characterFall[0][i];
		}
		for (int i = 0; i < 6; i++) {
			sprintf(path, "images/player/%s/land_%d.png", CHARACTER_FOLDERS[c], i + 1);
			characterLand[c][i] = iLoadImage(path);
			if (characterLand[c][i] == -1 && c == 0) {
				sprintf(legacyPath, "images/player/land_%d.png", i + 1);
				characterLand[c][i] = iLoadImage(legacyPath);
			}
			if (characterLand[c][i] == -1) characterLand[c][i] = characterLand[0][i];
		}
	}
}

void loadAllEnemySprites() {
	char path[256];
	for (int i = 0; i < 8; i++) {
		sprintf(path, "images/enemy/enemy_walk_right_%02d.png", i + 1);
		enemyWalkRight[i] = iLoadImage(path);
		sprintf(path, "images/enemy/enemy_walk_left_%02d.png", i + 1);
		enemyWalkLeft[i] = iLoadImage(path);
		sprintf(path, "images/enemy/enemy_shoot_right_%02d.png", i + 1);
		enemyShootRight[i] = iLoadImage(path);
		sprintf(path, "images/enemy/enemy_shoot_left_%02d.png", i + 1);
		enemyShootLeft[i] = iLoadImage(path);
	}
}

void iMouseMove(int mx, int my) {}
void iPassiveMouseMove(int mx, int my) {
	mouseX = mx;
	mouseY = my;
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (gameState == STATE_MENU) {
			float startX = 40;
			float btnW = 260, btnH = 45;
			if (mx >= startX && mx <= startX + btnW && my >= 415 && my <= 460) {
				triggerSoundMp3("music/ui_click.mp3");
				loadLevel(0);
			}
			else if (mx >= startX && mx <= startX + btnW && my >= 355 && my <= 400) {
				triggerSoundMp3("music/ui_click.mp3");
				gameState = STATE_LEVEL_SELECT;
			}
			else if (mx >= startX && mx <= startX + btnW && my >= 295 && my <= 340) {
				triggerSoundMp3("music/ui_click.mp3");
				gameState = STATE_CHARACTER_SELECT;
			}
			else if (mx >= startX && mx <= startX + btnW && my >= 235 && my <= 280) {
				triggerSoundMp3("music/ui_click.mp3");
				gameState = STATE_INSTRUCTIONS;
			}
			else if (mx >= startX && mx <= startX + btnW && my >= 175 && my <= 220) {
				triggerSoundMp3("music/ui_click.mp3");
				gameState = STATE_CREDITS;
			}
			else if (mx >= startX && mx <= startX + btnW && my >= 115 && my <= 160) {
				triggerSoundMp3("music/ui_click.mp3");
				exit(0);
			}
		}
		else if (gameState == STATE_LEVEL_SELECT) {
			for (int i = 0; i < TOTAL_LEVELS; i++) {
				float lx = 200 + (i % 3) * 300;
				float ly = 420 - (i / 3) * 160;
				if (mx >= lx && mx <= lx + 240 && my >= ly && my <= ly + 110 && levelUnlocked[i]) {
					triggerSoundMp3("music/ui_click.mp3");
					loadLevel(i);
					return;
				}
			}
			if (mx >= 50 && mx <= 190 && my >= 50 && my <= 90) {
				triggerSoundMp3("music/ui_click.mp3");
				goToMenu();
			}
		}
		else if (gameState == STATE_CHARACTER_SELECT) {
			if (mx >= 300 && mx <= 540 && my >= 300 && my <= 460) {
				selectedCharacter = 0;
				triggerSoundMp3("music/ui_click.mp3");
			}
			if (mx >= 660 && mx <= 900 && my >= 300 && my <= 460) {
				selectedCharacter = 1;
				triggerSoundMp3("music/ui_click.mp3");
			}
			if (mx >= 50 && mx <= 190 && my >= 40 && my <= 90) {
				triggerSoundMp3("music/ui_click.mp3");
				goToMenu();
			}
		}
		else if (gameState == STATE_LEVEL_CLEAR) {
			if (mx >= SCREEN_WIDTH / 2 - 110 && mx <= SCREEN_WIDTH / 2 + 110 && my >= 280 && my <= 325) {
				triggerSoundMp3("music/ui_click.mp3");
				loadLevel(currentLevelIndex + 1);
			}
			else if (mx >= SCREEN_WIDTH / 2 - 110 && mx <= SCREEN_WIDTH / 2 + 110 && my >= 210 && my <= 255) {
				triggerSoundMp3("music/ui_click.mp3");
				goToMenu();
			}
		}
		else if (gameState == STATE_GAME_VICTORY) {
			if (mx >= SCREEN_WIDTH / 2 - 110 && mx <= SCREEN_WIDTH / 2 + 110 && my >= 250 && my <= 295) {
				triggerSoundMp3("music/ui_click.mp3");
				goToMenu();
			}
		}
		else if (gameState == STATE_INSTRUCTIONS || gameState == STATE_CREDITS) {
			if (mx >= 50 && mx <= 190 && my >= 40 && my <= 90) {
				triggerSoundMp3("music/ui_click.mp3");
				goToMenu();
			}
		}
	}
}

void iKeyboard(unsigned char key) {
	if (key == 27) goToMenu();
}

void iSpecialKeyboard(unsigned char key) {}
void fixedUpdate() {}

int main() {
	initAllLevels();
	loadGameData();                 // ← Text File থেকে লোড

	iSetTimer(TIMER_INTERVAL_MS, updateGame);
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Loopbound: Paradox - Sci-Fi Time Loop Platformer");

	levelBackgroundID[0] = iLoadImage("images/bg1.jpg");
	levelBackgroundID[1] = iLoadImage("images/bg2.jpg");
	levelBackgroundID[2] = iLoadImage("images/bg3.jpg");
	levelBackgroundID[3] = iLoadImage("images/bg4.jpg");
	levelBackgroundID[4] = iLoadImage("images/bg5.jpg");

	loadAllCharacterSprites();
	loadAllEnemySprites();

	platformGrassImg = iLoadImage("images/platform_grass.png");

	for (int i = 0; i < 6; i++) {
		char path[128];
		sprintf(path, "images/flying_obj/bird_game_%d.png", i + 1);
		birdSprite[i] = iLoadImage(path);
	}

	menuBackgroundID = iLoadImage("images/menu_bg.jpg");
	levelSelectBackgroundID = iLoadImage("images/level_select_bg.jpg");
	instructionsBackgroundID = iLoadImage("images/how_to_play_bg.jpg");
	creditsBackgroundID = iLoadImage("images/credits_bg.jpg");

	goToMenu();
	iStart();
	return 0;
}