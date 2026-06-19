#pragma once

#include "text_renderer.h"
#include "constants.h"
#include "random_mt.h"

struct Menu
{
	TextRenderer* textRenderer{ nullptr };
	TextData menuTitle;
	std::array<SDL_FRect, BOARD_SIZE> backgroundTiles;
	
	Menu(TextRenderer* textRenderer) 
		:textRenderer(textRenderer)
	{
		menuTitle.text = "MINESWEEPER";
		menuTitle.anchor = { 0, 0 };
		menuTitle.horizontalAlignement = Alignement::CENTER;
		menuTitle.padding.top = HEIGHT / 4;
		menuTitle.pointSize = 64;

		for (int cell = 0; cell < BOARD_SIZE; cell++) {

		}
	}
	
	void Render(SDL_Renderer* renderer, Sprites& sprites) {
		
		for (int y = 0; y < BOARD_H; y++) {
			for (int x = 0; x < BOARD_W; x++) {
				SDL_FRect destination = {
					.x = static_cast<float>(x * CELL_SIZE * SPRITE_SCALE),
					.y = static_cast<float>(y * CELL_SIZE * SPRITE_SCALE),
					.w = CELL_SIZE * SPRITE_SCALE,
					.h = CELL_SIZE * SPRITE_SCALE
				};

				SDL_FRect sprite = sprites.GetSprite(random_mt::get(NUMBERS_OFFSET, NUMBERS_OFFSET + 8));
				SDL_RenderTexture(renderer, sprites.spriteSheet, &sprite, &destination);
			}
		}
		
		textRenderer->RenderText(menuTitle);
	}
};