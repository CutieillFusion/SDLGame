#include "AssetManager.h"
#include "Globals.h"

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}


void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}

void AssetManager::AddJSONFile(std::string id, const char* path)
{
	auto file = JSONParser(path);
	file.parse();
	files.emplace(id, file.root);
}

std::shared_ptr<JSON::JSONNode> AssetManager::GetJSONFile(std::string id)
{
	return files[id];
}

void AssetManager::LoadSpritesFromJSONFile(std::string id, std::string jsonId)
{
	auto root = GetJSONFile(id);

	auto spritePaths = root->returnObject()[jsonId]->returnObject()["path"]->returnList();
	auto spriteIds = root->returnObject()[jsonId]->returnObject()["id"]->returnList();

	for (int i = 0; i < spritePaths.size(); i++)
	{
		AddTexture(spriteIds[i]->returnString(), spritePaths[i]->returnString().c_str());
	}
}

