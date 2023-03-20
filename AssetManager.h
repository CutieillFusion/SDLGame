#pragma once

#include "SDL_ttf.h"
#include <map>
#include <vector>
#include <string>

class AssetManager
{
public:
	static AssetManager* instance;
	
	AssetManager();
	~AssetManager();

	//Texture Managing
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, const char* path, int fontSize);
	TTF_Font* GetFont(std::string id);

private:
	std::vector<std::string> textureIds;
	std::vector<SDL_Texture*> textures;
	std::vector<std::string> fontIds;
	std::vector<TTF_Font*> fonts;
};

