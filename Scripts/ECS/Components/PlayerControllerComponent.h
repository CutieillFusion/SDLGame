#pragma once
#include "..\ECS.h"
#include "..\..\Globals.h"
#include "InputListenerComponent.h"
#include "TransformComponent.h"

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