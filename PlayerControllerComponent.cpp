#include "PlayerControllerComponent.h"

void PlayerControllerComponent::Initialize()
{
	transform = &entity->getComponent<TransformComponent>();
	entity->AddTag("InputListenerComponent");
}

void PlayerControllerComponent::Update()
{
	float speed = 5.0f;
	Vector3D direction = Vector3D();

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

	transform->dPosition += direction.Normalize() * speed * (DELTA_TIME);
}

void PlayerControllerComponent::OnKeyDown(SDL_KeyboardEvent* key)
{
	if (key->repeat == 0 && key->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		if (0 < key->keysym.scancode && key->keysym.scancode <= 350)
		{
			keyboard[key->keysym.scancode] = true;
		}
	}
}

void PlayerControllerComponent::OnKeyUp(SDL_KeyboardEvent* key)
{
	if (key->repeat == 0 && key->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		if (0 < key->keysym.scancode && key->keysym.scancode <= 350)
		{
			keyboard[key->keysym.scancode] = false;
		}
	}
}

void PlayerControllerComponent::OnMouseDown(SDL_MouseButtonEvent* key)
{
}

void PlayerControllerComponent::OnMouseUp(SDL_MouseButtonEvent* key)
{
}
