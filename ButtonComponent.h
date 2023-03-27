#pragma once
#include "ECS.h"
#include "InputListenerComponent.h"
#include "UIColliderComponent.h"
#include "SpriteComponent.h"
#include <vector>

typedef void(*ButtonEvent)(SDL_MouseButtonEvent*);

class ButtonComponent : public InputListenerComponent
{
public:
	ButtonComponent() = default;
	ButtonComponent(ButtonEvent buttonEvent);
	ButtonComponent(std::vector<ButtonEvent> buttonEvents);

	void Initialize() override;
	void Update() override;

	void OnKeyDown(SDL_KeyboardEvent* key);
	void OnKeyUp(SDL_KeyboardEvent* key);
	void OnMouseDown(SDL_MouseButtonEvent* button);
	void OnMouseUp(SDL_MouseButtonEvent* button);

	void AddButtonEvent(ButtonEvent buttonEvent);
private:
	float mouseDown{};
	SpriteComponent* sprites{};
	UIColliderComponent* collider{};
	std::vector<ButtonEvent> buttonEvents;
};

