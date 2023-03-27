#pragma once
#include "ECS.h"
#include "SDL.h"

#define MAX_KEYBOARD_KEYS 350

class InputListenerComponent : public Component
{
public:
	virtual void OnKeyDown(SDL_KeyboardEvent* key) = 0;
	virtual void OnKeyUp(SDL_KeyboardEvent* key) = 0;
	virtual void OnMouseDown(SDL_MouseButtonEvent* mouse) = 0;
	virtual void OnMouseUp(SDL_MouseButtonEvent* mouse) = 0;
protected:
	bool keyboard[MAX_KEYBOARD_KEYS]{};
};