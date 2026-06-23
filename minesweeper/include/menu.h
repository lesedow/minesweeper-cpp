#pragma once

#include "text_system.h"
#include "constants.h"
#include "random_mt.h"

#include <SDL3/SDL.h>
#include <array>

#include "text_system.h"
#include "constants.h"
#include "sprites.h"

enum MenuState
{
	MAIN,
	DIFFICULTY
};

struct Menu
{
	MenuState state{ MenuState::MAIN };
	TextSystem* textSystem{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	
	SDL_FRect overlay{};

	TextData menuTitle{};

	TextData exitButtonText{};
	TextData playButtonText{};
	TextData easyDifficulty{};
	TextData mediumDifficulty{};
	TextData expertDifficulty{};

	std::array<int, constants::BOARD_SIZE> backgroundTilesIndexes{};
	
	bool Initialize(SDL_Renderer* renderer, TextSystem* textSystem);
	void Render(Sprites& sprites);
};