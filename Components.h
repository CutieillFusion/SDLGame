#pragma once
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "TextRendererComponent.h"
#include "PlayerControllerComponent.h"


class Collider : public Component
{
public:
	Collider() = default;
	Collider(std::vector<Vector3D> vertexs)
	{
		Collider::vertexs = vertexs;
	}

	void Initialize() override
	{

	}

	void Update() override
	{

	}

	void Render() override
	{
		SDL_Rect src = { 0,0, 4, 4 };
		SDL_Texture* texture = TextureManager::LoadTexture("Assets/debug.png");
		for (int i = 0; i < vertexs.size(); i++)
		{
			SDL_Rect dest = {vertexs[i].x * WORLD_SCALE, vertexs[i].y * WORLD_SCALE, src.w, src.h};
			TextureManager::Draw(texture, src, dest);
		}
	}

	std::vector<Vector3D> vertexs;
};
