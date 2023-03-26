#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "SDL_ttf.h"
#include "JSONParser.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();


	//Texture Management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	//Font Management
	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);

	//JSON Management
	void AddJSONFile(std::string id, const char* path);
	std::shared_ptr<JSON::JSONNode> GetJSONFile(std::string id);
	void LoadSpritesFromJSONFile(std::string id, std::string jsonId);

private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, std::shared_ptr<JSON::JSONNode>> files;
};