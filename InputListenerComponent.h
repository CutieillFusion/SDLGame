#pragma once
#include "ECS.h"
#include "SDL.h"

class InputListenerComponent : public Component
{
public:
	virtual void OnKeyDown(SDL_KeyboardEvent* key) = 0;
	virtual void OnKeyUp(SDL_KeyboardEvent* key) = 0;
protected:
	bool keyboard[MAX_KEYBOARD_KEYS];
};