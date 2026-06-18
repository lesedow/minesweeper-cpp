#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct TextRenderer
{
	SDL_Texture* fontTexture{ nullptr };
	TTF_TextEngine* textEngine{ nullptr };
	TTF_Font* font{ nullptr };
	
	bool LoadFont(std::string path);
	void RenderText(std::string text, SDL_Point position, int size);
};