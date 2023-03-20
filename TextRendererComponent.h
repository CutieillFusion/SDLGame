#pragma once
#include "ECS.h"
#include "Game.h"
#include "TextureManager.h"
#include "SDL_ttf.h"
#include "Vector.h"
#include "AssetManager.h"

class TextRendererComponent : public Component
{
public:
	SDL_Rect position;
	std::string text;
	TTF_Font* font;

	TextRendererComponent() = default;
	TextRendererComponent(std::string text, std::string id, Vector3D position, SDL_Color textColor)
	{
		TextRendererComponent::text = text;
		font = AssetManager::instance->GetFont(id);

		if (font == NULL)
		{
			std::cout << "FONT FAILED TO LOAD!..." << std::endl;
			return;
		}

		TextRendererComponent::position.x = position.x;
		TextRendererComponent::position.y = position.y;

		TextRendererComponent::textColor = textColor;

		SetText();
	}

	void SetText() 
	{
		surf = TTF_RenderText_Blended(font, text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void Initialize() override
	{

	}

	void Update() override
	{

	}

	void Render() override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}

private:
	SDL_Surface* surf;
	SDL_Color textColor;
	SDL_Texture* labelTexture;

};