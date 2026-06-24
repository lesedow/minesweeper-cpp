#include "text_system.h"
#include "constants.h"

bool TextSystem::Initialize(SDL_Renderer* renderer)
{
	if (!(textEngine = TTF_CreateRendererTextEngine(renderer))) {
		SDL_Log("Couldn't create text engine: %s", SDL_GetError());
		return false;
	}	

	return true;
}

TTF_Text* TextSystem::CreateText(std::string text)
{
	TTF_Text* ttfText = TTF_CreateText(textEngine, font, text.c_str(), 0);
	if (!ttfText) {
		SDL_Log("Failed to create text: %s", SDL_GetError());
	}
	return ttfText;
}

bool TextSystem::LoadFont(const std::filesystem::path& path)
{
	if (!(font = TTF_OpenFont(path.string().c_str(), constants::DEFAULT_FONT_SIZE))) {
		SDL_Log("Couldn't load specified font: %s", SDL_GetError());
		return false;
	};

	return true;
}

SDL_Point GetTextSize(TTF_Text* text)
{
	SDL_Point textSize{};
	TTF_GetTextSize(text, &textSize.x, &textSize.y);
	return textSize;
}

SDL_FPoint TextSystem::GetTransformedText(TextData& textData)
{
	SDL_FPoint calculatedPosition{};

	int textWidth, textHeight;
	if (!TTF_GetTextSize(textData.text, &textWidth, &textHeight)) {
		SDL_Log("Failed to get text size: %s", SDL_GetError());
	}

	switch (textData.horizontalAlignement) {
	case Alignement::RIGHT:
		calculatedPosition.x 
			= textData.anchor.x + 
			constants::WIDTH - textWidth + 
			textData.padding.right - textData.padding.left;
		break;
	case Alignement::LEFT:
		calculatedPosition.x 
			= textData.anchor.x +
			textData.padding.right - textData.padding.left;
		break;
	case Alignement::CENTER:
		calculatedPosition.x 
			= textData.anchor.x + 
			constants::WIDTH / 2 - textWidth / 2 + 
			textData.padding.right - textData.padding.left;
		break;
	}

	switch (textData.verticalAlignement) {
	case Alignement::TOP:
		calculatedPosition.y = 
			textData.anchor.y + 
			textData.padding.top - textData.padding.bottom;
		break;
	case Alignement::BOTTOM:
		calculatedPosition.y = textData.anchor.y + 
			constants::HEIGHT - textHeight + 
			textData.padding.top - textData.padding.bottom;
		break;
	case Alignement::CENTER:
		calculatedPosition.y = textData.anchor.y + 
			constants::HEIGHT / 2 - textHeight / 2 +
			textData.padding.top - textData.padding.bottom;
		break;
	}

	return calculatedPosition;
}

void TextSystem::RenderText(TextData& textData) {

	float ticks = SDL_GetTicks() / 1000.0f;
	const float speed = 2.0f;
	const float amplitude = 15.0f;

	if (!TTF_SetFontSize(font, textData.pointSize)) {
		SDL_Log("Failed to set font to specified size: %s", SDL_GetError());
	};
	TTF_SetFontStyle(font, textData.styleFlags);

	SDL_FPoint calculatedPosition = GetTransformedText(textData);
	if (textData.bobbing) {
		calculatedPosition.y += sin(ticks * speed) * amplitude;
	}
	
	if (textData.doubleText)
	{
		float offset = 5.0f;
		auto [red, green, blue, alpha] = textData.secondaryColor;
		if (!TTF_SetTextColor(textData.text, red, green, blue, alpha)) {
			SDL_Log("Failed to set text color: %s", SDL_GetError());
		};

		if (!TTF_DrawRendererText(textData.text, calculatedPosition.x + offset, calculatedPosition.y + offset)) {
			SDL_Log("Failed to render text: %s", SDL_GetError());
		}
	}

	auto [red, green, blue, alpha] = textData.primaryColor;
	TTF_SetTextColor(textData.text,	red, green, blue, alpha);

	if (!TTF_DrawRendererText(textData.text, calculatedPosition.x, calculatedPosition.y)) {
		SDL_Log("Failed to render text: %s", SDL_GetError());
	}
}