#pragma once

#include <cell.h>

#include <SDL3/SDL.h>
#include <array>

constexpr SDL_Color MENU_CLEAR_COLOR{ 192, 192, 192, 255 };

constexpr int SPRITESHEET_COLUMNS = 8;
constexpr int SPRITESHEET_ROWS = 2;
constexpr int CELL_SIZE = 16;
constexpr float SCALE = 2.0f;
constexpr int NUMBERS_OFFSET = 7;
constexpr size_t SPRITES_SOURCES_SIZE = SPRITESHEET_COLUMNS * SPRITESHEET_ROWS;

constexpr int BOARD_W = 30;
constexpr int BOARD_H = 16;
constexpr int WIDTH = CELL_SIZE * SCALE * BOARD_W;
constexpr int HEIGHT = CELL_SIZE * SCALE * BOARD_H;
constexpr int BOMB_COUNT = 99;

constexpr size_t BOARD_SIZE = BOARD_W * BOARD_H;
constexpr int NORMAL_CELLS_COUNT = BOARD_SIZE - BOMB_COUNT;