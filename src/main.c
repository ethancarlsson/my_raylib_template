#ifndef RAY_H
#define RAY_H
#include "raylib.h"
#endif

#ifndef STDIO_H
#define STDIO_H
#include "stdio.h"
#endif

#include "game.h"
#include "platform.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main(void) {
	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(1000, 800, "The game");
	SetTargetFPS(60);
	char text[200]; // in the top left
	struct GameState gamesState = {
	    text,
	};
	//--------------------------------------------------------------------------------------
	// Main loop
	//--------------------------------------------------------------------------------------
#if defined(PLATFORM_WEB)
	typedef void (*myFuncDef)(void *);
	myFuncDef cb = &gameTick;
	emscripten_set_main_loop_arg(cb, &gamesState, 0, 1);
#else
	while (!WindowShouldClose()) {
		gameTick(&gamesState);
	}
#endif
	//--------------------------------------------------------------------------------------
	// Close resources
	CloseWindow();
	//--------------------------------------------------------------------------------------
	return 0;
}
