#pragma once
#include "ECS.h"
#include "Vector.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include "AssetManager.h"

class ImageRendererComponent : public Component
{
public:
	ImageRendererComponent() = default;
	ImageRendererComponent(Vector3D position, Vector3D scale);

	void Initialize() override;
	void Update() override;
	void Render() override;

	SpriteComponent* sprites;
	Vector3D position;
	Vector3D scale;
	SDL_FRect destRect;
};

