#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "Vector.h"
#include "AssetManager.h"

class TileComponent : public Component
{
public:
	SDL_Texture* texture;

	TileComponent() = default;
	TileComponent(std::string id);

	void Initialize() override;
	void Update() override;
	void Render() override;

private:
	TransformComponent* transform;
	SDL_Rect srcRect;
};