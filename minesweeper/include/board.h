#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <vector>
#include <algorithm>
#include <random>

#include "cell.h"
#include "constants.h"

struct Board
{
    std::array<Cell, constants::BOARD_SIZE> board;
    bool isFirstClick = true;
    int flagsPlaced = 0;
    int revealedCells = 0;

    Cell& GetCell(int index);
    SDL_Point GetCellPositionByIndex(int index);
    void HandleLeftClickedCell(int pressedCellIndex);
    void HandleRightClickedCell(int pressedCellIndex);
    void RevealBombs();
    void RecursiveFill(int pressedCellIndex);
    void PlaceMines(int index);
    void CountCells();
    void RevealCell(int pressedCellIndex);
    int GetCellIndexFromScreenCoordinates(float mouseX, float mouseY);
    bool IsCellNeighbours(std::vector<int>& neighbours, int cellIndex);
    std::vector<int> GetNeighboursOfCell(SDL_Point cellPosition);
    std::vector<int> GetRevealableCellsIndexes(int pressedCellIndex);
    std::vector<int> GetShuffledBoardIndexes(SDL_Point pressedCellPosition);
};