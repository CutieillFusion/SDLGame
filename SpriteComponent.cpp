#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(std::string id)
{
	textures.emplace_back(Game::assets->GetTexture(id));
}

SpriteComponent::SpriteComponent(std::vector<std::string> ids)
{
	for (std::string id : ids)
	{
		textures.emplace_back(Game::assets->GetTexture(id));
	}
}

void SpriteComponent::Initialize()
{
}

void SpriteComponent::Update()
{
}

void SpriteComponent::Render()
{
}

void SpriteComponent::AddTexture(std::string id)
{
	textures.emplace_back(Game::assets->GetTexture(id));
}

void SpriteComponent::AddTexture(std::vector<std::string> ids)
{
	for (std::string id : ids)
	{
		textures.emplace_back(Game::assets->GetTexture(id));
	}
}
