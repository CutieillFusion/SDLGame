#pragma once
#include "ECS.h"
#include "SDL.h"
#include "Vector.h"
#include "InputListenerComponent.h"
#include "TransformComponent.h"

#define DELTA_TIME 1.0f / 144.0f
#define MAX_KEYBOARD_KEYS 350

class PlayerControllerComponent : public InputListenerComponent
{
public:
	PlayerControllerComponent() = default;

	void Initialize() override;
	void Update() override;

	void OnKeyDown(SDL_KeyboardEvent* key);
	void OnKeyUp(SDL_KeyboardEvent* key);
	void OnMouseDown(SDL_MouseButtonEvent* key);
	void OnMouseUp(SDL_MouseButtonEvent* key);

private:
	TransformComponent* transform{};
};