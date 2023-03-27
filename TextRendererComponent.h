#pragma once
#include "ECS.h"
#include "AssetManager.h"
#include "SDL_ttf.h"
#include "Vector.h"
#include "RectComponent.h"

class TextRendererComponent : public Component
{
public:
	RectComponent* rect;
	std::string text;
	TTF_Font* font;

	TextRendererComponent() = default;
	TextRendererComponent(std::string text, std::string id, SDL_Color textColor);

	void SetText();

	void Initialize() override;
	void Update() override;
	void Render() override;

private:
	SDL_Surface* surf;
	SDL_Color textColor;
	SDL_Rect position;
	SDL_Texture* labelTexture;

};