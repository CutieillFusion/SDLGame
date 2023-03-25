#pragma once
#include "ECS.h"
#include "Vector.h"
#include "SDL.h"
#include "AssetManager.h"

class ImageRendererComponent : public Component
{
public:
	ImageRendererComponent() = default;
	ImageRendererComponent(std::string id, Vector3D position, Vector3D scale);

	void Initialize() override;
	void Update() override;
	void Render() override;

	SDL_FRect destRect;
	SDL_Texture* texture;
	Vector3D position;
	Vector3D scale;
};

