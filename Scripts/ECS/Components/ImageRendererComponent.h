#pragma once
#include "RectComponent.h"
#include "SpriteComponent.h"
#include "..\ECS.h"
#include "..\..\Globals.h"
#include "..\..\AssetManager.h"
class ImageRendererComponent : public Component
{
public:
	ImageRendererComponent() = default;

	void Initialize() override;
	void Update() override;
	void Render() override;

	SpriteComponent* sprites{};
	RectComponent* rect{};
	SDL_FRect destRect{};
};

