#pragma once
#include "ECS.h"
#include "SDL.h"
#include "Vector.h"
#include "TransformComponent.h"

#define DELTA_TIME 1.0f / 144.0f

class PlayerControllerComponent : public Component
{
public:
	PlayerControllerComponent() = default;

	void Initialize() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void Update() override
	{
		float speed = 5.0f;
		Vector2D direction = Vector2D();

		if (keyboard[SDL_SCANCODE_W])
		{
			direction.y -= 1;
		}

		if (keyboard[SDL_SCANCODE_A])
		{
			direction.x -= 1;
		}

		if (keyboard[SDL_SCANCODE_S])
		{
			direction.y += 1;
		}

		if (keyboard[SDL_SCANCODE_D])
		{
			direction.x += 1;
		}

		transform->position += direction.Normalize() * speed * (DELTA_TIME);
	}

	void Render() override
	{

	}

	void KeyDown(SDL_KeyboardEvent* key)
	{
		if (key->repeat == 0 && key->keysym.scancode < MAX_KEYBOARD_KEYS)
		{
			if (0 < key->keysym.scancode && key->keysym.scancode <= 350)
			{
				keyboard[key->keysym.scancode] = true;
			}
		}
	}

	void KeyUp(SDL_KeyboardEvent* key)
	{
		if (key->repeat == 0 && key->keysym.scancode < MAX_KEYBOARD_KEYS)
		{
			if (0 < key->keysym.scancode && key->keysym.scancode <= 350)
			{
				keyboard[key->keysym.scancode] = false;
			}
		}
	}

private:
	TransformComponent* transform;
	bool keyboard[MAX_KEYBOARD_KEYS];
};