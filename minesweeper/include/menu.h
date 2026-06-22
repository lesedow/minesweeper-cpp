#pragma once

#include "text_renderer.h"
#include "constants.h"
#include "random_mt.h"

struct Menu
{
	TextRenderer* textRenderer{ nullptr };
	TextData menuTitle;
	std::array<int, constants::BOARD_SIZE> backgroundTiles{};
	
	Menu(TextRenderer* textRenderer) 
		:textRenderer(textRenderer)
	{
		menuTitle.text = "MINESWEEPER";
		menuTitle.anchor = { 0, 0 };
		menuTitle.horizontalAlignement = Alignement::CENTER;
		menuTitle.padding.top = constants::HEIGHT / 4;
		menuTitle.pointSize = 64;

		for (int cell = 0; cell < constants::BOARD_SIZE; cell++) {

		}
	}
	
	void Render(SDL_Renderer* renderer, Sprites& sprites) {
		
		for (int y = 0; y < constants::BOARD_H; y++) {
			for (int x = 0; x < constants::BOARD_W; x++) {
				SDL_FRect destination = {
					.x = static_cast<float>(x * constants::CELL_SIZE * constants::SPRITE_SCALE),
					.y = static_cast<float>(y * constants::CELL_SIZE * constants::SPRITE_SCALE),
					.w = constants::CELL_SIZE * constants::SPRITE_SCALE,
					.h = constants::CELL_SIZE * constants::SPRITE_SCALE
				};

				int randomIndex = random_mt::get(
					constants::NUMBERS_OFFSET,
					constants::NUMBERS_OFFSET + 8
				);
				
				SDL_FRect sprite = sprites.GetSprite(randomIndex);
				SDL_RenderTexture(renderer, sprites.spriteSheet, &sprite, &destination);
			}
		}
		
		textRenderer->RenderText(menuTitle);
	}
};