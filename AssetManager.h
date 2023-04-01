#pragma once

#include "TextureManager.h"
#include "JSONParser.h"
#include "PokemonObject.h"
#include "PokemonManager.h"
#include "PokemonBattleManager.h"
#include "Globals.h"

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

	//Pokemon Management
	void LoadMovesFromJSONFile(std::string id, std::string jsonId);
	PokemonMoveObject* GetMove(std::string id);
	void LoadPokemonsFromJSONFile(std::string id, std::string jsonId);
	PokemonObject* GetPokemon(std::string id);

	//JSON Management
	void AddJSONFile(std::string id, const char* path);
	std::shared_ptr<JSON::JSONNode> GetJSONFile(std::string id);
	void LoadSpritesFromJSONFile(std::string id, std::string jsonId);

	PokemonManager* pokemonManager;
	PokemonBattleManager* pokemonBattleManager;
private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, std::shared_ptr<JSON::JSONNode>> files;
};