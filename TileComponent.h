#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Vector.h"
#include "AssetManager.h"

class TileComponent : public Component
{
public:
	TileComponent() = default;

	void Initialize() override;
	void Update() override;
	void Render() override;

private:
	SpriteComponent* sprites;
	TransformComponent* transform;
	SDL_Rect srcRect;
};