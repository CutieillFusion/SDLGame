#include "ButtonEvents.h"
#include "Game.h"
#include "ECS.h"

void TestEvents::Test(SDL_MouseButtonEvent* button)
{
	if (button->type == SDL_MOUSEBUTTONDOWN)
	{
		//std::cout << "BUTTON DOWN" << std::endl;
	}
}

void TestEvents::Disable(SDL_MouseButtonEvent* button)
{
	if (button->type == SDL_MOUSEBUTTONUP) 
	{
		auto manager = Game::manager;
		auto entities = manager->FindEntitiesWithTag("Button");
		if (entities.size() > 0) 
		{
			manager->entities[entities[0]].get()->SetActiveStatus(false);
		}
	}
}
