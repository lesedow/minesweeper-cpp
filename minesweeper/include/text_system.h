#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <string_view>
#include <filesystem>

#include "constants.h"

enum Alignement
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	CENTER
};

struct Padding
{
	float left{ 0 };
	float right{ 0 };
	float top{ 0 };
	float bottom{ 0 };
};
	
struct TextData
{
	Padding padding{};
	Alignement horizontalAlignement{ Alignement::LEFT };
	Alignement verticalAlignement{ Alignement::TOP };
	SDL_FPoint anchor{};

	TTF_FontStyleFlags styleFlags{ TTF_STYLE_NORMAL };
	SDL_Color primaryColor;
	SDL_Color secondaryColor;

	TTF_Text* text{ nullptr };
	float pointSize{ constants::DEFAULT_FONT_SIZE };
	bool bobbing{ false };
	bool doubleText{ false };
};

struct TextSystem
{
	TTF_TextEngine* textEngine{ nullptr };
	TTF_Font* font{ nullptr };
	
	bool Initialize(SDL_Renderer* renderer);
	bool LoadFont(const std::filesystem::path& path);
	SDL_FPoint GetTransformedText(TextData& textData);
	void RenderText(TextData& textData);
	TTF_Text* CreateText(std::string text);
	SDL_Point GetTextSize(TTF_Text* text);
};
