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

void TextRenderer::RenderText(TextData textData) {
	if (!TTF_SetFontSize(font, textData.pointSize)) {
		SDL_Log("Failed to set font to specified size: %s", SDL_GetError());
	};

	TTF_Text* ttfText = TTF_CreateText(textEngine, font, textData.text.c_str(), 0);
	if (!ttfText) {
		SDL_Log("Failed to create text: %s", SDL_GetError());
	}

	int textWidth, textHeight;
	TTF_GetTextSize(ttfText, &textWidth, &textHeight);

	textData.anchor.x = WIDTH / 2 - textWidth / 2;
	textData.anchor.y = HEIGHT / 4;
	// SDL_Log("%f %f", textData.anchor.x, textData.anchor.y);

	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	TTF_DrawRendererText(ttfText, textData.anchor.x, textData.anchor.y);
}