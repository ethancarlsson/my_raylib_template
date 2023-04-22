#ifndef RAY_H
#define RAY_H
#include "raylib.h"
#endif

#ifndef STDIO_H
#define STDIO_H
#include "stdio.h"
#endif

#include "game.h"

void gameTick(void *arg) {
	struct GameState *gamesState = (struct GameState *) arg;

	BeginDrawing();

	ClearBackground(DARKGRAY);

	sprintf(gamesState->text, "A game about distributing resources...");

	DrawText(gamesState->text, 10, 30, 20, LIGHTGRAY);

	EndDrawing();
}
