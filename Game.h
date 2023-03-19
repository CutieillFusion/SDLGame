#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

#define WORLD_SCALE 32
#define MAX_KEYBOARD_KEYS 350
#define DELTA_TIME 1.0f / 144.0f

class Game
{
public:

	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

	void UpdateCollisions();

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; }

	static SDL_Renderer* renderer;
private:
	bool isRunning;
	SDL_Window *window;
};