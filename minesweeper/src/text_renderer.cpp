#include "text_renderer.h"
#include "constants.h"

bool TextRenderer::Initialize(SDL_Renderer* renderer)
{
	if (!(engine = TTF_CreateRendererTextEngine(renderer))) {
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
}

void TextRenderer::RenderText(std::string text, SDL_Position position, int size) {
	if (!TTF_SetFontSize(font, size)) {
		SDL_Log("Failed to set font to specified size: %s", SDL_GetError());
	};

	TTF_Text* text = TTF_CreateText(textEngine, font, text.c_str(), 0);
}