#pragma once

#include <SDL3/SDL.h>
#include <format>
#include <filesystem>
#include <cstddef>
#include <string_view>

namespace constants
{
	// Colors
	inline constexpr SDL_Color MENU_TITLE_PRIMARY = { 0xEC, 0xEC, 0xEC, 0xFF };
	inline constexpr SDL_Color MENU_TITLE_SECONDARY = { 0xC0, 0xC0, 0xC0, 0xFF };

	// Assets paths
	inline constexpr std::string_view SPRITE_SHEET_LOCATION = "assets/sprites/minesweeper_sprites.png";
	inline constexpr std::string_view ROBOTO_CONDENSED_LOCATION = "assets/fonts/Roboto_Condensed-Regular.ttf";

	inline const std::filesystem::path SPRITE_SHEET_PATH = std::filesystem::current_path() / SPRITE_SHEET_LOCATION;
	inline const std::filesystem::path ROBOTO_CONDENSED_PATH = std::filesystem::current_path() / ROBOTO_CONDENSED_LOCATION;

	// Font default settings
	inline constexpr float DEFAULT_FONT_SIZE = 16.0f;

	// Sprites default settings
	inline constexpr int CELL_SIZE = 16;
	inline constexpr int SPRITESHEET_COLUMNS = 8;
	inline constexpr int SPRITESHEET_ROWS = 2;
	inline constexpr float SPRITE_SCALE = 2.0f;
	inline constexpr int NUMBERS_OFFSET = 7;
	
	inline constexpr std::size_t SPRITES_SOURCES_SIZE = SPRITESHEET_COLUMNS * SPRITESHEET_ROWS;

	// Board default settings
	inline constexpr int BOARD_W = 30;
	inline constexpr int BOARD_H = 16;
	inline constexpr int WIDTH = CELL_SIZE * SPRITE_SCALE * BOARD_W;
	inline constexpr int HEIGHT = CELL_SIZE * SPRITE_SCALE * BOARD_H;
	inline constexpr int BOMB_COUNT = 99;

	inline constexpr std::size_t BOARD_SIZE = BOARD_W * BOARD_H;

	inline constexpr int NORMAL_CELLS_COUNT = BOARD_SIZE - BOMB_COUNT;
}