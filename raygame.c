#include "raylib.h"
#include "stdio.h"
#include "constants.h"

// #define PLATFORM_WEB

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

struct PaddlePositions {
	int left;
	int right;
};

void positionsUdateWithKeyPress(struct PaddlePositions *positions) {
	int increment = BALL_SPEED; // how fast it goes
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
		ballDir->x = BALL_SPEED;
		int yDir = (((double) ballPos->y - (double) positions->left) / PADDLE_SIZE) * 100;
		ballDir->y = (yDir - 50) / BALL_SPEED;
	}
	else if (isOnRightSide && ballPos->y >= positions->right && ballPos->y <= positions->right + PADDLE_SIZE) {
		ballDir->x = -BALL_SPEED;
		int yDir = (((double) ballPos->y - (double) positions->right) / PADDLE_SIZE) * 100;
		ballDir->y = (yDir - 50) / BALL_SPEED;
	}
	return true;
}

struct GameState {
	const Music *music;
	const Sound *collisionSound;
	struct PaddlePositions *positions;
	struct BallPosition *ballPos;
	struct BallPosition *ballDir;
	char *text;
	char *winnerText;
	bool isGameOver;
	int volleys;
	int framesCounter;
};

void gameTick(void *arg) {
	struct GameState *gamesState = (struct GameState *) arg;

	UpdateMusicStream(*gamesState->music);
	BeginDrawing();

	ClearBackground(DARKPURPLE);

	if (gamesState->ballPos->x > SCREENWIDTH) {
		sprintf(gamesState->winnerText, "Left wins!");
		gamesState->isGameOver = true;
	}

	if (gamesState->ballPos->x < 0) {
		sprintf(gamesState->winnerText, "Right wins!");
		gamesState->isGameOver = true;
	}

	if (gamesState->ballPos->y > SCREENHEIGHT || gamesState->ballPos->y < 0) {
		gamesState->ballDir->y = gamesState->ballDir->y * -1;
	}

	int keyPressed = GetKeyPressed();

	sprintf(gamesState->text, "Volleys %d", gamesState->volleys);

	if (gamesState->isGameOver && keyPressed == RESTART) {
		gamesState->ballPos->x = SCREENWIDTH / 2;
		gamesState->ballPos->y = SCREENHEIGHT / 2;
		gamesState->ballDir->x = 5;
		gamesState->ballDir->y = 0;
		gamesState->volleys = 0;
		gamesState->isGameOver = false;
		gamesState->positions->left = (SCREENHEIGHT / 2) - (PADDLE_SIZE / 2);
		gamesState->positions->right = (SCREENHEIGHT / 2) - (PADDLE_SIZE / 2);
		gamesState->framesCounter = 0;
	}

	if (gamesState->isGameOver) {
		if (gamesState->framesCounter < 2000) {
			gamesState->framesCounter += 8;
		}

		sprintf(
		    gamesState->text,
		    "Game over! %s\nPress r to restart\n%d volleys\n\n\nMusic credit goes to:\nsyzmalix: https://linktr.ee/szymalix\nand kablazic: https://soundcloud.com/kablazik\n\nWritting using Raylib",
		    gamesState->winnerText,
		    gamesState->volleys - 1);
		EndDrawing();
		DrawText(TextSubtext(gamesState->text, 0, gamesState->framesCounter / 10), 10, 30, 20, LIGHTGRAY);

		return;
	}

	DrawText(gamesState->text, 10, 30, 20, LIGHTGRAY);

	if (collisionsChangeBallDirection(gamesState->ballDir, gamesState->ballPos, gamesState->positions)) {
		PlaySound(*gamesState->collisionSound);
		gamesState->volleys += 1;
	};

	gamesState->ballPos->x += gamesState->ballDir->x;
	gamesState->ballPos->y += gamesState->ballDir->y;
	positionsUdateWithKeyPress(gamesState->positions);

	DrawRectangle(LEFT_PADDLE_XPOS, gamesState->positions->left, 10, PADDLE_SIZE, BLUE);
	DrawRectangle(RIGHT_PADDLE_XPOS, gamesState->positions->right, 10, PADDLE_SIZE, RED);
	DrawCircle(gamesState->ballPos->x, gamesState->ballPos->y, 10, LIGHTGRAY);

	EndDrawing();
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

	struct GameState gamesState = {
	    &music,
	    &collisionSound,
	    &positions,
	    &ballPos,
	    &ballDir,
	    text,
	    winnerText,
	    isGameOver,
	    volleys,
	    framesCounter,
	};

#if defined(PLATFORM_WEB)
	typedef void (*myFuncDef)(void *);
	myFuncDef cb = &gameTick;
	emscripten_set_main_loop_arg(cb, &gamesState, 0, 1);
#else
	while (!WindowShouldClose()) {
		gameTick(&gamesState);
	}
#endif

	CloseWindow();
	CloseAudioDevice();
	UnloadSound(collisionSound); // probably not necessary here, but good to know we can do it
	UnloadMusicStream(music);

	return 0;
}
