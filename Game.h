#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>
#include "Vector.h"
#include "ECS.h"

#define WORLD_SCALE 32
#define MAX_KEYBOARD_KEYS 350
#define DELTA_TIME 1.0f / 144.0f

class AssetManager;

class Game
{
public:

	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

	//Deals with Updates that envolve other systems
	void UpdateCollisions();
	void UpdateUICollisions();

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; }

	static SDL_Renderer* renderer;
	static Vector2D camera;
	static AssetManager* assets;
	static Manager* manager;
private:
	Vector2D newCamera;
	SDL_Rect mousePos{};
	bool isRunning{};
	SDL_Window* window{};

};