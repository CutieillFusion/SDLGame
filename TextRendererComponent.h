#pragma once
#include "ECS.h"
#include "Game.h"
#include "TextureManager.h"
#include "SDL_ttf.h"
#include "Vector.h"

class TextRendererComponent : public Component
{
public:
	SDL_Rect position;
	std::string text;
	TTF_Font* font;

	TextRendererComponent() = default;
	TextRendererComponent(std::string text, const char* path, Vector2D position, SDL_Color textColor)
	{
		TextRendererComponent::text = text;
		font = TTF_OpenFont(path, 32);

		if (font == NULL)
		{
			std::cout << "FONT FAILED TO LOAD!..." << std::endl;
			return;
		}

		TextRendererComponent::position.x = position.x;
		TextRendererComponent::position.y = position.y;

		TextRendererComponent::textColor = textColor;
		UpdateText();
	}

	void Initialize() override
	{

	}

	void UpdateText()
	{
		SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void Update() override
	{

	}

	void Render() override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}

private:
	SDL_Color textColor;
	SDL_Texture* labelTexture;

};