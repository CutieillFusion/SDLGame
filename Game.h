#pragma once

#include "Globals.h"
#include "ECS.h"
#include "Vector.h"

class AssetManager;

class Game
{
public:

	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
	void InitializeDaeemon();

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
	Vector2D screenSize{};
	Vector2D newCamera;
	SDL_Rect mousePos{};
	bool isRunning{};
	SDL_Window* window{};

};