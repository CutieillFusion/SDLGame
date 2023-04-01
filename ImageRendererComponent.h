#pragma once
#include "ECS.h"
#include "Globals.h"
#include "SpriteComponent.h"
#include "AssetManager.h"
#include "RectComponent.h"
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

