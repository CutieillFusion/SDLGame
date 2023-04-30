#pragma once
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "..\ECS.h"
#include "..\..\Globals.h"
#include "..\..\AssetManager.h"

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