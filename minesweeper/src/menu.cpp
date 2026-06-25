#include "menu.h"
#include "random_mt.h"

bool Menu::Initialize(SDL_Renderer* renderer, TextSystem* textSystem)
{
	if (!renderer || !textSystem) return false;

	this->renderer = renderer;
	this->textSystem = textSystem;

	overlay.w = constants::WIDTH;
	overlay.h = constants::HEIGHT;

	menuTitle.anchor = { 0, 0 };
	menuTitle.horizontalAlignement = Alignement::CENTER;
	menuTitle.padding.top = constants::HEIGHT / 4;
	menuTitle.pointSize = 64;
	menuTitle.bobbing = true;
	menuTitle.doubleText = true;
	menuTitle.primaryColor = constants::MENU_TITLE_PRIMARY;
	menuTitle.secondaryColor = constants::MENU_TITLE_SECONDARY;
	menuTitle.styleFlags = TTF_STYLE_BOLD | TTF_STYLE_ITALIC;	
	
	playButtonText.anchor = { 0, 0 };
	playButtonText.horizontalAlignement = Alignement::CENTER;
	playButtonText.verticalAlignement = Alignement::BOTTOM;
	playButtonText.padding.bottom = constants::HEIGHT / 4;
	playButtonText.pointSize = 32;
	playButtonText.primaryColor = constants::MENU_TITLE_PRIMARY;
	playButtonText.styleFlags = TTF_STYLE_BOLD | TTF_STYLE_ITALIC;

	exitButtonText.anchor = { 0, 0 };
	exitButtonText.horizontalAlignement = Alignement::CENTER;
	exitButtonText.verticalAlignement = Alignement::BOTTOM;
	exitButtonText.padding.bottom = constants::HEIGHT / 4 - playButtonText.pointSize;
	exitButtonText.padding.top = 25.0f;
	exitButtonText.pointSize = 32;
	exitButtonText.primaryColor = constants::MENU_TITLE_PRIMARY;
	exitButtonText.styleFlags = TTF_STYLE_BOLD | TTF_STYLE_ITALIC;

	easyDifficulty.anchor = { 0, 0 };
	easyDifficulty.horizontalAlignement = Alignement::CENTER;
	easyDifficulty.padding.top = constants::HEIGHT / 4;
	easyDifficulty.pointSize = 32;
	easyDifficulty.primaryColor = constants::MENU_TITLE_PRIMARY;
	easyDifficulty.styleFlags = TTF_STYLE_BOLD | TTF_STYLE_ITALIC;

	mediumDifficulty.anchor = { 0, 0 };
	mediumDifficulty.horizontalAlignement = Alignement::CENTER;
	mediumDifficulty.padding.top = easyDifficulty.padding.top + 100.0f;
	mediumDifficulty.pointSize = 32;
	mediumDifficulty.primaryColor = constants::MENU_TITLE_PRIMARY;
	mediumDifficulty.styleFlags = TTF_STYLE_BOLD | TTF_STYLE_ITALIC;
	
	expertDifficulty.anchor = { 0, 0 };
	expertDifficulty.horizontalAlignement = Alignement::CENTER;
	expertDifficulty.padding.top = mediumDifficulty.padding.top + 100.0f;;
	expertDifficulty.pointSize = 32;
	expertDifficulty.primaryColor = constants::MENU_TITLE_PRIMARY;
	expertDifficulty.styleFlags = TTF_STYLE_BOLD | TTF_STYLE_ITALIC;

	menuTitle.text = textSystem->CreateText("MINESWEEPER", menuTitle.pointSize, menuTitle.styleFlags);
	playButtonText.text = textSystem->CreateText("PLAY", playButtonText.pointSize, playButtonText.styleFlags);
	exitButtonText.text = textSystem->CreateText("EXIT", exitButtonText.pointSize, exitButtonText.styleFlags);

	easyDifficulty.text = textSystem->CreateText("EASY", easyDifficulty.pointSize, easyDifficulty.styleFlags);
	mediumDifficulty.text = textSystem->CreateText("MEDIUM", mediumDifficulty.pointSize, mediumDifficulty.styleFlags);
	expertDifficulty.text = textSystem->CreateText("EXPERT", expertDifficulty.pointSize, expertDifficulty.styleFlags);
	
	for (int cell = 0; cell < backgroundTilesIndexes.size(); cell++) {
		int randomIndex = random_mt::get(0, constants::NUMBERS_OFFSET * 2);
		backgroundTilesIndexes[cell] = randomIndex;
	}

	return true;
}

void Menu::Render(Sprites& sprites) {

	for (int y = 0; y < constants::EXPERT_H; y++) {
		for (int x = 0; x < constants::EXPERT_W; x++) {
			SDL_FRect destination = {
				.x = static_cast<float>(x * constants::CELL_SIZE * constants::SPRITE_SCALE),
				.y = static_cast<float>(y * constants::CELL_SIZE * constants::SPRITE_SCALE),
				.w = constants::CELL_SIZE * constants::SPRITE_SCALE,
				.h = constants::CELL_SIZE * constants::SPRITE_SCALE
			};

			int index = backgroundTilesIndexes[x + y * constants::EXPERT_W];
			SDL_FRect sprite = sprites.GetSprite(index);
			SDL_RenderTexture(renderer, sprites.spriteSheet, &sprite, &destination);
		}
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
	SDL_RenderFillRect(renderer, &overlay);
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

	if (state == MenuState::MAIN) {
		textSystem->RenderText(menuTitle);
		textSystem->RenderText(playButtonText);
		textSystem->RenderText(exitButtonText);
	}
	else if (state == MenuState::DIFFICULTY) {
		textSystem->RenderText(easyDifficulty);
		textSystem->RenderText(mediumDifficulty);
		textSystem->RenderText(expertDifficulty);
	}
}