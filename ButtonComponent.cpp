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
		sprites->SetIndex(1);
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
	if (collider->colliding)
	{
		mouseDown = 0.25f;
		sprites->SetIndex(2);
		
		for (auto entityAction : entityActions) 
		{
			entityAction.entity->SetActiveStatus(entityAction.activeStatus);
		}

		for (auto buttonEvent : buttonEvents)
		{
			(buttonEvent)(button);
		}

		if (!entity->isActive()) 
		{
			mouseDown = 0;
			collider->colliding = false;
		}
	}
}

void ButtonComponent::OnMouseUp(SDL_MouseButtonEvent* button)
{
	
}

void ButtonComponent::AddEntityAction(EntityAction action)
{
	entityActions.emplace_back(action);
}

void ButtonComponent::AddEntityActions(std::vector<EntityAction> actions)
{
	entityActions.insert(entityActions.end(), actions.begin(), actions.end());
}

void ButtonComponent::AddButtonEvent(ButtonEvent buttonEvent)
{
	buttonEvents.emplace_back(buttonEvent);
}

void ButtonComponent::AddButtonEvents(std::vector<ButtonEvent> buttonEvents)
{
	ButtonComponent::buttonEvents.insert(ButtonComponent::buttonEvents.end(), buttonEvents.begin(), buttonEvents.end());
}
