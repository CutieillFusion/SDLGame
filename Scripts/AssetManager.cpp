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

Daemon::Model::Attack* AssetManager::CreateMove(std::string id)
{
	auto root = GetJSONFile("DaemonMoves")->returnObject()[id];

	auto name = id;
	auto power = (int)root->returnObject()["Power"]->returnFloat();
	auto type = ReturnType(root->returnObject()["Type"]->returnString());
	auto accuracy = (int)root->returnObject()["Accuracy"]->returnFloat();
	auto special = root->returnObject()["Special"]->returnBool();
	auto status = root->returnObject()["Status"]->returnBool();
	auto criticalRate = (int)root->returnObject()["CriticalRate"]->returnFloat();
	auto neverFails = root->returnObject()["NeverFails"]->returnBool();
	auto ppMax = (int)root->returnObject()["PPMax"]->returnFloat();
	auto priority = (int)root->returnObject()["Priority"]->returnFloat();
	auto preEffect = ReturnAttackEffect(root->returnObject()["PreEffect"]->returnString());
	auto postEffect = ReturnAttackEffect(root->returnObject()["PostEffect"]->returnString());
	auto failEffect = ReturnAttackEffect(root->returnObject()["FailEffect"]->returnString());

	return new Daemon::Model::Attack(name, power, type, accuracy, special, status, criticalRate, neverFails, ppMax, priority, preEffect, postEffect, failEffect);
}

Daemon::Model::Species* AssetManager::CreateSpecies(std::string id)
{
	auto root = GetJSONFile("DaemonSpecies")->returnObject()[id];

	auto name = id;
	auto baseAttack = (int)root->returnObject()["BaseAttack"]->returnFloat();
	auto baseDefense = (int)root->returnObject()["BaseDefense"]->returnFloat();
	auto baseSpecialAttack = (int)root->returnObject()["BaseSpecialAttack"]->returnFloat();
	auto baseSpecialDefense = (int)root->returnObject()["BaseSpecialDefense"]->returnFloat();
	auto baseSpeed = (int)root->returnObject()["BaseSpeed"]->returnFloat();
	auto baseHealth = (int)root->returnObject()["BaseHealth"]->returnFloat();
	auto type1 = ReturnType(root->returnObject()["Type1"]->returnString());
	auto type2 = ReturnType(root->returnObject()["Type2"]->returnString());
	auto evolutionType = nullptr;//Implement Later
	auto evsObject = root->returnObject()["EVsGiven"]->returnList();
	std::vector<std::string> evsString;
	for (auto evs : evsObject) { evsString.emplace_back(evs->returnString()); }
	auto evsGiven = ReturnEVsGiven(evsString);
	auto height = root->returnObject()["Height"]->returnFloat();
	auto weight = root->returnObject()["Weight"]->returnFloat();
	auto daedexEntry = root->returnObject()["DaedexEntry"]->returnString();
	auto expGiven = (int)root->returnObject()["ExpGiven"]->returnFloat();
	auto expMax = (int)root->returnObject()["ExpMax"]->returnFloat();
	auto captureRate = (int)root->returnObject()["CaptureRate"]->returnFloat();
	auto daedexNumber = (int)root->returnObject()["DaedexNumber"]->returnFloat();

	return new Daemon::Model::Species(baseAttack, baseDefense, baseSpecialAttack, baseSpecialDefense, baseSpeed, baseHealth, name, type1, type2, evolutionType, evsGiven, height, weight, daedexEntry, expGiven, expMax, captureRate, daedexNumber);
}

Daemon::Model::DaeTeam* AssetManager::CreateDaeTeam(std::string id)
{
	auto root = GetJSONFile("DaemonTeams")->returnObject()[id];

	Daemon::Model::DaeTeam* team = new Daemon::Model::DaeTeam(id);
	auto teamSize = (int)root->returnObject()["TeamSize"]->returnFloat();

	for (int i = 1; i <= teamSize; i++)
	{
		auto daemonRoot = root->returnObject()["Daemon" + std::to_string(i)];
		auto nickname = daemonRoot->returnObject()["Nickname"]->returnString();
		auto species = CreateSpecies(daemonRoot->returnObject()["Species"]->returnString());
		auto level = (int)daemonRoot->returnObject()["Level"]->returnFloat();
		std::vector<Daemon::Model::Attack*> moves;
		auto moveList = daemonRoot->returnObject()["Moves"]->returnList();
		for (int j = 0; j < moveList.size(); j++)
		{
			moves.emplace_back(CreateMove(moveList[j]->returnString()));
		}
		auto nature = ReturnNature(daemonRoot->returnObject()["Nature"]->returnString());
		auto ivs = daemonRoot->returnObject()["IVs"]->returnList();

		auto daemon = new Daemon::Model::Daemon(nickname, species, level, moves, nature);
		//Sets IVs
		daemon->SetHPIV((int)ivs[0]->returnFloat());
		daemon->SetAtkIV((int)ivs[1]->returnFloat());
		daemon->SetDefIV((int)ivs[2]->returnFloat());
		daemon->SetAtkSpeIV((int)ivs[3]->returnFloat());
		daemon->SetDefSpeIV((int)ivs[4]->returnFloat());
		daemon->SetSpeIV((int)ivs[5]->returnFloat());
		daemon->CalcStats();
		daemon->Heal(daemon->GetStatHP());
		team->AddDaeMon(daemon);
	}

	return team;
}

Daemon::Model::AttackEffect* AssetManager::ReturnAttackEffect(std::string name) 
{
	if (name == "nullptr") { return nullptr; }
	else if (name == "LowerDefense -1") { return new Daemon::Model::Attacks::ChangeStatEffect(Daemon::Model::Attacks::ChangeStatEffect::Target::DEFENDER, Daemon::Model::Stats::DEF, -1); }
	else if (name == "LowerAttack -1") { return new Daemon::Model::Attacks::ChangeStatEffect(Daemon::Model::Attacks::ChangeStatEffect::Target::DEFENDER, Daemon::Model::Stats::ATK, -1); }
	else if (name == "Protect") { return new Daemon::Model::Attacks::ProtectEffect(); }
	return nullptr;
}

Daemon::Model::Type AssetManager::ReturnType(std::string name)
{
	if (name == "Neutral") { return Daemon::Model::Type::NEUTRAL; }
	else if (name == "Liquid") { return Daemon::Model::Type::LIQUID; }
	else if (name == "Bad") { return Daemon::Model::Type::BAD; }
	else if (name == "Bug") { return Daemon::Model::Type::BUG; }
	else if (name == "Burning") { return Daemon::Model::Type::BURNING; }
	else if (name == "Cold") { return Daemon::Model::Type::COLD; }
	else if (name == "Dragon") { return Daemon::Model::Type::DRAGON; }
	else if (name == "Electron") { return Daemon::Model::Type::ELECTRON; }
	else if (name == "Fight") { return Daemon::Model::Type::FIGHT; }
	else if (name == "Ghost") { return Daemon::Model::Type::GHOST; }
	else if (name == "Ground") { return Daemon::Model::Type::GROUND; }
	else if (name == "Magic") { return Daemon::Model::Type::MAGIC; }
	else if (name == "Mental") { return Daemon::Model::Type::MENTAL; }
	else if (name == "Mineral") { return Daemon::Model::Type::MINERAL; }
	else if (name == "Nothing") { return Daemon::Model::Type::NOTHING; }
	else if (name == "Sky") { return Daemon::Model::Type::SKY; }
	else if (name == "Toxic") { return Daemon::Model::Type::TOXIC; }
	else if (name == "Vegetal") { return Daemon::Model::Type::VEGETAL; }

	std::cout << "SHOULDN\'T BE HERE | AssetManager::ReturnType" << std::endl;
	return Daemon::Model::Type::NOTHING;
}

std::vector<Daemon::Model::Stats> AssetManager::ReturnEVsGiven(std::vector<std::string> evs)
{
	std::vector<Daemon::Model::Stats> evsGiven;

	for (auto ev : evs) 
	{
		if (ev == "Health") { evsGiven.emplace_back(Daemon::Model::Stats::HP); }
		else if (ev == "Attack") { evsGiven.emplace_back(Daemon::Model::Stats::ATK); }
		else if (ev == "Defense") { evsGiven.emplace_back(Daemon::Model::Stats::DEF); }
		else if (ev == "Special Attack") { evsGiven.emplace_back(Daemon::Model::Stats::ATKSPE); }
		else if (ev == "Special Defense") { evsGiven.emplace_back(Daemon::Model::Stats::DEFSPE); }
		else if (ev == "Speed") { evsGiven.emplace_back(Daemon::Model::Stats::SPE); }
	}

	return evsGiven;
}

Daemon::Model::Nature AssetManager::ReturnNature(std::string name)
{
	if (name == "Bold") { return Daemon::Model::Nature::BOLD; }
	else if (name == "Bashful") { return Daemon::Model::Nature::BASHFUL; }
	else if (name == "Brave") { return Daemon::Model::Nature::BRAVE; }
	else if (name == "Calm") { return Daemon::Model::Nature::CALM; }
	else if (name == "Quiet") { return Daemon::Model::Nature::QUIET; }
	else if (name == "Docile") { return Daemon::Model::Nature::DOCILE; }
	else if (name == "Mild") { return Daemon::Model::Nature::MILD; }
	else if (name == "Rash") { return Daemon::Model::Nature::RASH; }
	else if (name == "Gentle") { return Daemon::Model::Nature::GENTLE; }
	else if (name == "Hardy") { return Daemon::Model::Nature::HARDY; }
	else if (name == "Jolly") { return Daemon::Model::Nature::JOLLY; }
	else if (name == "Lax") { return Daemon::Model::Nature::LAX; }
	else if (name == "Impish") { return Daemon::Model::Nature::IMPISH; }
	else if (name == "Sassy") { return Daemon::Model::Nature::SASSY; }
	else if (name == "Naughty") { return Daemon::Model::Nature::NAUGHTY; }
	else if (name == "Modest") { return Daemon::Model::Nature::MODEST; }
	else if (name == "Naive") { return Daemon::Model::Nature::NAIVE; }
	else if (name == "Hasty") { return Daemon::Model::Nature::HASTY; }
	else if (name == "Careful") { return Daemon::Model::Nature::CAREFUL; }
	else if (name == "Quirky") { return Daemon::Model::Nature::QUIRKY; }
	else if (name == "Relaxed") { return Daemon::Model::Nature::RELAXED; }
	else if (name == "Adamant") { return Daemon::Model::Nature::ADAMANT; }
	else if (name == "Serious") { return Daemon::Model::Nature::SERIOUS; }
	else if (name == "Lonely") { return Daemon::Model::Nature::LONELY; }
	else if (name == "Timid") { return Daemon::Model::Nature::TIMID; }

	std::cout << "SHOULDN\'T BE HERE | AssetManager::ReturnNature" << std::endl;
	return Daemon::Model::Nature::BOLD;
}
