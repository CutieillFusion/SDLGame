#pragma once
#include "Game.h"

class Map
{
public:

	Map();
	~Map();

	void LoadMap(int arr[20][25]);
	void DrawMap();
private:

	SDL_Rect src, dest;
	SDL_Texture* textures[3];

	int map[20][25];
};
