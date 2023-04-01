#pragma once
#include "ECS.h"
#include "Globals.h"

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