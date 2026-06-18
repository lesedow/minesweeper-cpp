#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "constants.h"

enum Alignement
{
	TOP,
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
	float pointSize{ DEFAULT_FONT_SIZE };
	SDL_FPoint anchor{};
	std::string text{};
};

struct TextRenderer
{
	TTF_TextEngine* textEngine{ nullptr };
	TTF_Font* font{ nullptr };
	
	bool Initialize(SDL_Renderer* renderer);
	bool LoadFont(std::string path);
	void RenderText(TextData textData);
};