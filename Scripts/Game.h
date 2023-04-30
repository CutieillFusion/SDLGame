#pragma once

#include "Globals.h"
#include "ECS\ECS.h"
#include "Vector.h"

class AssetManager;

class Game
{
public:

	Game();
	~Game();

	void Initialize(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

	//Deals with Updates that envolve other systems
	void UpdateCollisions();
	void UpdateUICollisions();

	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool Running() { return isRunning; }

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