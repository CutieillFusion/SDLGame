#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(ButtonEvent buttonEvent)
{
	AddButtonEvent(buttonEvent);
}

ButtonComponent::ButtonComponent(std::vector<ButtonEvent> buttonEvents) : buttonEvents(buttonEvents)
{

}

void ButtonComponent::Initialize()
{
	entity->AddTag("InputListenerComponent");
	sprites = &entity->getComponent<SpriteComponent>();
	collider = &entity->getComponent<UIColliderComponent>();
}

void ButtonComponent::Update()
{
	mouseDown -= DELTA_TIME;
	if (collider->colliding && mouseDown < 0) 
	{
		sprites->SetIndex(2);
	}
	else 
	{
		if (mouseDown < 0) 
		{
			sprites->SetIndex(0);
		}
	}
}

void ButtonComponent::OnKeyDown(SDL_KeyboardEvent* key)
{
}

void ButtonComponent::OnKeyUp(SDL_KeyboardEvent* key)
{
}

void ButtonComponent::OnMouseDown(SDL_MouseButtonEvent* button)
{
	mouseDown = 0.25f;
	sprites->SetIndex(1);
	for (auto buttonEvent : buttonEvents) 
	{
		(buttonEvent)(button);
	}
}

void ButtonComponent::OnMouseUp(SDL_MouseButtonEvent* button)
{
	for (auto buttonEvent : buttonEvents)
	{
		(buttonEvent)(button);
	}
}

void ButtonComponent::AddButtonEvent(ButtonEvent buttonEvent)
{
	buttonEvents.emplace_back(buttonEvent);
}
