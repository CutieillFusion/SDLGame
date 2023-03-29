#pragma once
#include "ECS.h"
#include "InputListenerComponent.h"
#include "UIColliderComponent.h"
#include "SpriteComponent.h"
#include <vector>

struct EntityAction
{
	Entity* entity;
	bool activeStatus;
};

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

	void AddEntityAction(EntityAction action);
	void AddEntityActions(std::vector<EntityAction> actions);
	void AddButtonEvent(ButtonEvent buttonEvent);
	void AddButtonEvents(std::vector<ButtonEvent> buttonEvents);

private:
	float mouseDown{};
	SpriteComponent* sprites{};
	UIColliderComponent* collider{};
	std::vector<ButtonEvent> buttonEvents;
	std::vector<EntityAction> entityActions;
};

