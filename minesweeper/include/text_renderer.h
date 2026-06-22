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
	TTF_FontStyleFlags styleFlags{};
	TTF_Text* text{ nullptr };
	bool bobbing{ false };
};

struct TextRenderer
{
	TTF_TextEngine* textEngine{ nullptr };
	TTF_Font* font{ nullptr };
	
	bool Initialize(SDL_Renderer* renderer);
	bool LoadFont(const std::filesystem::path& path);
	TTF_Text* CreateText(std::string_view text);
	void RenderText(TextData& textData);
};