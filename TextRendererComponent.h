#pragma once
#include "ECS.h"
#include "AssetManager.h"
#include "Globals.h"
#include "RectComponent.h"

enum TEXT_HORIZONTAL_ALIGNMENT
{
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT
};

enum TEXT_VERTICAL_ALIGNMENT
{
	TEXT_ALIGN_TOP,
	TEXT_ALIGN_MIDDLE,
	TEXT_ALIGN_BOTTOM
};

class TextRendererComponent : public Component
{
public:
	RectComponent* rect;
	std::string text;
	TTF_Font* font;

	TextRendererComponent() = default;
	TextRendererComponent(std::string text, std::string id, SDL_Color textColor, TEXT_HORIZONTAL_ALIGNMENT textHorizontalAlignment, TEXT_VERTICAL_ALIGNMENT textVerticalAlignment);

	void SetText();
	void SetText(std::string text);

	void Initialize() override;
	void Update() override;
	void Render() override;

	TEXT_HORIZONTAL_ALIGNMENT textHorizontalAlignment;
	TEXT_VERTICAL_ALIGNMENT textVerticalAlignment;
private:
	SDL_Surface* surf;
	SDL_Color textColor;
	SDL_Rect position;
	SDL_Texture* labelTexture;

};