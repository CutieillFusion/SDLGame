#include "TextRendererComponent.h"

TextRendererComponent::TextRendererComponent(std::string text, std::string id, SDL_Color textColor)
{
	TextRendererComponent::text = text;
	font = Game::assets->GetFont(id);

	if (font == NULL)
	{
		std::cout << "FONT FAILED TO LOAD!..." << std::endl;
		return;
	}

	TextRendererComponent::textColor = textColor;

	SetText();
}

void TextRendererComponent::SetText()
{
	surf = TTF_RenderText_Blended(font, text.c_str(), textColor);
	labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
	SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
}

void TextRendererComponent::Initialize()
{
	rect = &entity->getComponent<RectComponent>();
	TextRendererComponent::position.x = (int)rect->position.x;
	TextRendererComponent::position.y = (int)rect->position.y;
}

void TextRendererComponent::Update()
{
}

void TextRendererComponent::Render()
{
	SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
}
