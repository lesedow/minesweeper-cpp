#pragma once

#include <cell.h>

#include <array>
#include <ranges>

using namespace std::ranges;
using namespace std::ranges::views;

constexpr int SPRITESHEET_COLUMNS = 5;
constexpr int SPRITESHEET_ROWS = 3;
constexpr int CELL_SIZE = 16;
constexpr float SCALE = 4.0f;
constexpr size_t SPRITES_SOURCES_SIZE = SPRITESHEET_COLUMNS * SPRITESHEET_ROWS;

constexpr int BOARD_W = 30;
constexpr int BOARD_H = 16;
constexpr int WIDTH = CELL_SIZE * SCALE * BOARD_W;
constexpr int HEIGHT = CELL_SIZE * SCALE * BOARD_H;
constexpr int BOMB_COUNT = 99;

constexpr size_t BOARD_SIZE = BOARD_W * BOARD_H;
constexpr int NORMAL_CELLS_COUNT = BOARD_SIZE - BOMB_COUNT;

constexpr std::array<CellVisual, 8> numbersVisuals = {
    ONE_CELL, TWO_CELL,
    THREE_CELL, FOUR_CELL, FIVE_CELL,
    SIX_CELL, SEVEN_CELL, EIGHT_CELL
};