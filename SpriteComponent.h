#pragma once
#include "ECS.h"
#include <vector>
#include "SDL.h"
#include "AssetManager.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent() = default;
	SpriteComponent(std::string id);
	SpriteComponent(std::vector<std::string> ids);

	void Initialize() override;
	void Update() override;
	void Render() override;

	void AddTexture(std::string id);
	void AddTexture(std::vector<std::string> ids);

	SDL_Texture* GetCurrentTexture();

	void SetIndex(unsigned int index);
	unsigned int GetIndex();

private:
	unsigned int index = 0;
	std::vector<SDL_Texture*> textures;
};

