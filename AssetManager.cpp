#include "AssetManager.h"
#include "Globals.h"

AssetManager::AssetManager()
{
	pokemonManager = new PokemonManager();
	pokemonBattleManager = new PokemonBattleManager();
}

AssetManager::~AssetManager()
{}


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

void AssetManager::LoadMovesFromJSONFile(std::string id, std::string jsonId)
{
	auto root = GetJSONFile(id);
	
	auto names = root->returnObject()[jsonId]->returnObject()["Name"]->returnList();
	auto types = root->returnObject()[jsonId]->returnObject()["Type"]->returnList();
	auto powers = root->returnObject()[jsonId]->returnObject()["Power"]->returnList();
	auto maxPowerPoints = root->returnObject()[jsonId]->returnObject()["MaxPowerPoints"]->returnList();
	auto accuracies = root->returnObject()[jsonId]->returnObject()["Accuracy"]->returnList();

	for (int i = 0; i < names.size(); i++)
	{
		PokemonType type = NORMAL;
		if (types[i]->returnString() == "Normal") 
		{
			type = NORMAL;
		}
		else if (types[i]->returnString() == "Fire")
		{
			type = FIRE;
		}
		else if (types[i]->returnString() == "Water") 
		{
			type = WATER;
		}
		else if (types[i]->returnString() == "Grass")
		{
			type = GRASS;
		}

		pokemonManager->AddMove(names[i]->returnString(), type, (int)powers[i]->returnFloat(), (int)maxPowerPoints[i]->returnFloat(), (int)accuracies[i]->returnFloat());
	}
}

PokemonMoveObject* AssetManager::GetMove(std::string id)
{
	return pokemonManager->GetMove(id);
}

void AssetManager::LoadPokemonsFromJSONFile(std::string id, std::string jsonId)
{
	auto root = GetJSONFile(id);
	
	auto names = root->returnObject()[jsonId]->returnObject()["Name"]->returnList();
	auto types = root->returnObject()[jsonId]->returnObject()["Types"]->returnList();
	auto baseStats = root->returnObject()[jsonId]->returnObject()["BaseStats"]->returnList();
	auto spriteIds = root->returnObject()[jsonId]->returnObject()["SpriteId"]->returnList(); 

	for (int i = 0; i < names.size(); i++)
	{
		std::vector<PokemonType> _types;
		for (int j = 0; j < types[i]->returnList().size(); j++)
		{
			if (types[i]->returnList()[j]->returnString() == "Normal")
			{
				_types.emplace_back(NORMAL);
			}
			else if (types[i]->returnList()[j]->returnString() == "Fire")
			{
				_types.emplace_back(FIRE);
			}
			else if (types[i]->returnList()[j]->returnString() == "Water")
			{
				_types.emplace_back(WATER);
			}
			else if (types[i]->returnList()[j]->returnString() == "Grass")
			{
				_types.emplace_back(GRASS);
			}
		}
		
		PokemonStats _baseStats = PokemonStats(
			(int)baseStats[i]->returnList()[0]->returnFloat(),
			(int)baseStats[i]->returnList()[1]->returnFloat(),
			(int)baseStats[i]->returnList()[2]->returnFloat(),
			(int)baseStats[i]->returnList()[3]->returnFloat(),
			(int)baseStats[i]->returnList()[4]->returnFloat(),
			(int)baseStats[i]->returnList()[5]->returnFloat()
		);


		pokemonManager->AddPokemonObject(names[i]->returnString(), _types, _baseStats, names[i]->returnString());
	}
}

PokemonObject* AssetManager::GetPokemon(std::string id)
{
	return nullptr;
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

