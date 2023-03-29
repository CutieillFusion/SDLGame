#include "TextRendererComponent.h"

TextRendererComponent::TextRendererComponent(std::string text, std::string id, SDL_Color textColor, TEXT_HORIZONTAL_ALIGNMENT textHorizontalAlignment, TEXT_VERTICAL_ALIGNMENT textVerticalAlignment)
{
	TextRendererComponent::text = text;
	TextRendererComponent::textHorizontalAlignment = textHorizontalAlignment;
	TextRendererComponent::textVerticalAlignment = textVerticalAlignment;

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

	if (textHorizontalAlignment == TEXT_ALIGN_RIGHT) 
	{
		position.x -= -rect->scale.x + position.w;
	}
	else if (textHorizontalAlignment == TEXT_ALIGN_CENTER) 
	{
		position.x -= -rect->scale.x + position.w / 2.0f;
	}

	if (textVerticalAlignment == TEXT_ALIGN_BOTTOM)
	{
		position.y -= -rect->scale.y + position.h;
	}
	else if (textVerticalAlignment == TEXT_ALIGN_CENTER)
	{
		position.y -= -rect->scale.y / 2.0f + position.h / 2.0f;
	}
}

void TextRendererComponent::Update()
{
	TextRendererComponent::position.x = (int)rect->position.x;
	TextRendererComponent::position.y = (int)rect->position.y;

	if (textHorizontalAlignment == TEXT_ALIGN_RIGHT)
	{
		position.x -= -rect->scale.x + position.w;
	}
	else if (textHorizontalAlignment == TEXT_ALIGN_CENTER)
	{
		position.x -= -rect->scale.x / 2.0f + position.w / 2.0f;
	}

	if (textVerticalAlignment == TEXT_ALIGN_BOTTOM)
	{
		position.y -= -rect->scale.y + position.h;
	}
	else if (textVerticalAlignment == TEXT_ALIGN_CENTER)
	{
		position.y -= -rect->scale.y / 2.0f + position.h / 2.0f;
	}
}

void TextRendererComponent::Render()
{
	SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
}
