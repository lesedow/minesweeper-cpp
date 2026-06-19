#include "text_renderer.h"
#include "constants.h"

bool TextRenderer::Initialize(SDL_Renderer* renderer)
{
	if (!(textEngine = TTF_CreateRendererTextEngine(renderer))) {
		SDL_Log("Couldn't create text engine: %s", SDL_GetError());
		return false;
	}	

	return true;
}

bool TextRenderer::LoadFont(std::string path)
{
	if (!(font = TTF_OpenFont(path.c_str(), DEFAULT_FONT_SIZE))) {
		SDL_Log("Couldn't load specified font: %s", SDL_GetError());
		return false;
	};

	return true;
}

void TextRenderer::RenderText(TextData& textData) {
	if (!TTF_SetFontSize(font, textData.pointSize)) {
		SDL_Log("Failed to set font to specified size: %s", SDL_GetError());
	};

	TTF_Text* ttfText = TTF_CreateText(textEngine, font, textData.text.c_str(), 0);
	if (!ttfText) {
		SDL_Log("Failed to create text: %s", SDL_GetError());
	}

	int textWidth, textHeight;
	TTF_GetTextSize(ttfText, &textWidth, &textHeight);
	
	SDL_FPoint textActualPosition{ 0, 0 };

	switch (textData.horizontalAlignement) {
	case Alignement::RIGHT:
		textActualPosition.x = textData.anchor.x + WIDTH - textWidth; //+ textData.padding.right - textData.padding.left;
		break;
	case Alignement::LEFT:
		textActualPosition.x = textData.anchor.x;// + textData.padding.right - textData.padding.left;
		break;
	case Alignement::CENTER:
		textActualPosition.x = textData.anchor.x + WIDTH / 2 - textWidth / 2;// +textData.padding.right - textData.padding.left;
		break;
	}

	switch (textData.verticalAlignement) {
	case Alignement::TOP:
		textActualPosition.y = textData.anchor.y + textData.padding.top - textData.padding.bottom;
		break;
	case Alignement::BOTTOM:
		textActualPosition.y = textData.anchor.y;// +textData.padding.right - textData.padding.left;
		break;
	case Alignement::CENTER:
		textActualPosition.y = textData.anchor.y + textHeight / 2;// +textData.padding.bottom - textData.padding.top;
		break;
	}

	textActualPosition.y += sin(SDL_GetTicks() / 1000.0f * 2.0f) * 25.0f;

	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	TTF_SetTextColor(ttfText, 0, 0, 0, 255);
	TTF_DrawRendererText(ttfText, textActualPosition.x, textActualPosition.y);
}