#pragma once

#include "TextureManager.h"
#include "Globals.h"
#include "..\Scripts\JSON Loader\JSONParser.h"
#include "..\Scripts\Daemon\Daemon.h"
#include "..\Scripts\Daemon\Attacks.h"
#include "..\Scripts\Daemon\DaemonTeam.h"
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

	Daemon::Model::Attack* CreateMove(std::string id);
	Daemon::Model::Species* CreateSpecies(std::string id);
	Daemon::Model::DaeTeam* CreateDaeTeam(std::string id);
private:
	//Pokemon Management
	Daemon::Model::AttackEffect* ReturnAttackEffect(std::string name);
	Daemon::Model::Type ReturnType(std::string name);
	std::vector<Daemon::Model::Stats> ReturnEVsGiven(std::vector<std::string> evs);
	Daemon::Model::Nature ReturnNature(std::string name);

	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, std::shared_ptr<JSON::JSONNode>> files;
};