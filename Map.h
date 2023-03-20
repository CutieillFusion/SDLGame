#pragma once
#include "Game.h"
#include "ECS.h"

class Map
{
public:

	Map(Manager* manager);
	~Map();

	void LoadMap(int arr[20][25]);

private:
	Manager* manager;
	SDL_Rect src, dest;
	std::string textures[3];

	int map[20][25];
};
