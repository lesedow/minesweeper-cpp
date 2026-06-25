#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <vector>
#include <algorithm>
#include <random>

#include "cell.h"
#include "constants.h"

enum BoardState
{
    NONE,
    WIN,
    LOSE
};

struct Board
{
    std::vector<Cell> board;
    bool isFirstClick{ true };

    int flagsPlaced{};
    int revealedCells{};
    
    int boardWidth{};
    int boardHeight{};
    int bombs{};

    void Reset();
    void InitializeBoard(int boardW, int boardH, int bombsCount);
    Cell& GetCell(int index);
    SDL_Point GetCellPositionByIndex(int index);

    BoardState HandleLeftClickedCell(int pressedCellIndex);
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