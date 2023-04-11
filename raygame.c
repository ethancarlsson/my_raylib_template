#include "raylib.h"
#include "stdio.h"

const int ARROW_UP = 264;
const int ARROW_DOWN = 265;

const int WASD_UP = 87;
const int WASD_DOWN = 83;

const int RESTART = 82;

const int PADDLE_SIZE = 80;

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;

const int LEFT_PADDLE_XPOS = 0 + 20;
const int RIGHT_PADDLE_XPOS = SCREENWIDTH - 20;

struct PaddlePositions {
	int left;
	int right;
};

void positionsUdateWithKeyPress(struct PaddlePositions *positions) {
	int increment = 5; // how fast it goes
	if (IsKeyDown(ARROW_UP)) {
		if (positions->right + PADDLE_SIZE < SCREENHEIGHT) {
			positions->right += increment;
		}
	}
	else if (IsKeyDown(ARROW_DOWN)) {

		if (positions->right > 0) {
			positions->right -= increment;
		}
	}

	if (IsKeyDown(WASD_UP)) {
		if (positions->left > 0) {
			positions->left -= increment;
		}
	}
	else if (IsKeyDown(WASD_DOWN)) {
		if (positions->left + PADDLE_SIZE < SCREENHEIGHT) {
			positions->left += increment;
		}
	}
}

struct BallPosition {
	int x;
	int y;
};

// changes direction of ball based on colision. Returns true if collides
bool collisionsChangeBallDirection(struct BallPosition *ballDir, const struct BallPosition *ballPos, const struct PaddlePositions *positions) {
	bool isOnLeftSide = ballPos->x == LEFT_PADDLE_XPOS;
	bool isOnRightSide = ballPos->x == RIGHT_PADDLE_XPOS;
	if (!isOnLeftSide && !isOnRightSide) {
		return false;
	}

	if (isOnLeftSide && ballPos->y >= positions->left && ballPos->y <= positions->left + PADDLE_SIZE) {
		ballDir->x = 5;
		int yDir = (((double) ballPos->y - (double) positions->left) / PADDLE_SIZE) * 100;
		ballDir->y = (yDir - 50) / 5;
	}
	else if (isOnRightSide && ballPos->y >= positions->right && ballPos->y <= positions->right + PADDLE_SIZE) {
		ballDir->x = -5;
		int yDir = (((double) ballPos->y - (double) positions->right) / PADDLE_SIZE) * 100;
		ballDir->y = (yDir - 50) / 5;
	}
	return true;
}

int main(void) {
	// Initialization
	//--------------------------------------------------------------------------------------

	InitWindow(SCREENWIDTH, SCREENHEIGHT, "Paddle Game");

	SetTargetFPS(60); // Set our game to run at 60 frames-per-second

	char text[200]; // in the top left
	char winnerText[12];
	sprintf(winnerText, "Right wins!");

	struct PaddlePositions positions = {
	    (SCREENHEIGHT / 2) - (PADDLE_SIZE / 2),
	    (SCREENHEIGHT / 2) - (PADDLE_SIZE / 2),
	};

	struct BallPosition ballPos = {
	    SCREENWIDTH / 2,
	    SCREENHEIGHT / 2,
	};

	struct BallPosition ballDir = {
	    5,
	    0,
	};

	bool isGameOver = false;
	int volleys = 0;
	int framesCounter = 0;

	// Audio
	InitAudioDevice();
	Sound collisionSound = LoadSound("./collision.wav");
	Music music = LoadMusicStream("./song_loop.wav");
	PlayMusicStream(music);
	SetMusicVolume(music, 0.1); // Set volume for music (1.0 is max level)

	while (!WindowShouldClose()) {
		UpdateMusicStream(music);
		BeginDrawing();

		ClearBackground(DARKPURPLE);

		if (ballPos.x > SCREENWIDTH) {
			sprintf(winnerText, "Left wins!");
			isGameOver = true;
		}

		if (ballPos.x < 0) {
			sprintf(winnerText, "Right wins!");
			isGameOver = true;
		}

		if (ballPos.y > SCREENHEIGHT || ballPos.y < 0) {
			ballDir.y = ballDir.y * -1;
		}

		int keyPressed = GetKeyPressed();

		sprintf(text, "Volleys %d", volleys);

		if (isGameOver && keyPressed == RESTART) {
			ballPos.x = SCREENWIDTH / 2;
			ballPos.y = SCREENHEIGHT / 2;
			ballDir.x = 5;
			ballDir.y = 0;
			volleys = 0;
			isGameOver = false;
			positions.left = (SCREENHEIGHT / 2) - (PADDLE_SIZE / 2);
			positions.right = (SCREENHEIGHT / 2) - (PADDLE_SIZE / 2);
			framesCounter = 0;
		}

		if (isGameOver) {
			if (framesCounter < 2000) {
				framesCounter += 8;
			}

			sprintf(text, "Game over! %s\nPress r to restart\n%d volleys\n\n\nMusic credit goes to:\nsyzmalix: https://linktr.ee/szymalix\nand kablazic: https://soundcloud.com/kablazik\n\nWritting using Raylib", winnerText, volleys - 1);
			EndDrawing();
			DrawText(TextSubtext(text, 0, framesCounter / 10), 10, 30, 20, LIGHTGRAY);

			continue;
		}

		DrawText(text, 10, 30, 20, LIGHTGRAY);

		if (collisionsChangeBallDirection(&ballDir, &ballPos, &positions)) {
			PlaySound(collisionSound);
			volleys += 1;
		};

		ballPos.x += ballDir.x;
		ballPos.y += ballDir.y;
		positionsUdateWithKeyPress(&positions);

		DrawRectangle(LEFT_PADDLE_XPOS, positions.left, 10, PADDLE_SIZE, BLUE);
		DrawRectangle(RIGHT_PADDLE_XPOS, positions.right, 10, PADDLE_SIZE, RED);
		DrawCircle(ballPos.x, ballPos.y, 10, LIGHTGRAY);

		EndDrawing();
	}

	CloseWindow();
	CloseAudioDevice();
	UnloadSound(collisionSound); // probably not necessary here, but good to know we can do it
	UnloadMusicStream(music);

	return 0;
}
