#include "board.h"
#include "random_mt.h"
#include "constants.h"

#include <ranges>

using namespace std::ranges::views;
using namespace std::ranges;

void Board::Reset()
{
    flagsPlaced = 0;
    revealedCells = 0;
    isFirstClick = true;
}

void Board::InitializeBoard(int boardW, int boardH, int bombsCount)
{
    boardWidth = boardW;
    boardHeight = boardH;

    board = std::vector<Cell>(boardW * boardH);
    bombs = std::clamp(bombsCount, 0, boardW * boardH - constants::EXCLUDED_CELLS_COUNT); 
}

Cell& Board::GetCell(int index)
{
    return board.at(index);
}

SDL_Point Board::GetCellPositionByIndex(int index)
{
    return {
        .x = index % boardWidth,
        .y = index / boardWidth
    };
}

BoardState Board::HandleLeftClickedCell(int pressedCellIndex)
{
    if (isFirstClick) {
        isFirstClick = false;
        PlaceMines(pressedCellIndex);
        CountCells();
    }

    Cell& pressedCell = GetCell(pressedCellIndex);

    if (pressedCell.isBomb) {
        RevealBombs();
        return BoardState::LOSE;
    }

    RevealCell(pressedCellIndex);

    if (revealedCells == boardWidth * boardHeight - bombs) {
        return BoardState::WIN;
    }

    return BoardState::NONE;
}

void Board::HandleRightClickedCell(int pressedCellIndex)
{
    Cell& cell = GetCell(pressedCellIndex);
    flagsPlaced += cell.ToggleFlag() ? 1 : -1;
}

std::vector<int> Board::GetRevealableCellsIndexes(int pressedCellIndex)
{
    SDL_Point position = GetCellPositionByIndex(pressedCellIndex);
    Cell& pressedCell = GetCell(pressedCellIndex);

    std::vector<int> neighbours = GetNeighboursOfCell(position);

    int flagged = count_if(neighbours, [&](int index) {
        Cell& neighbour = GetCell(index);
        return neighbour.state == CellState::FLAGGED;
        });

    auto revealable = neighbours | filter([&](int index) {
        Cell& neighbour = GetCell(index);
        return neighbour.state != CellState::FLAGGED &&
            neighbour.state != CellState::SHOWN;
        });

    return flagged == pressedCell.adjacentNeighbours
        ? revealable | to<std::vector>()
        : std::vector<int>();
}

void Board::RevealCell(int pressedCellIndex)
{
    Cell& target = GetCell(pressedCellIndex);

    if (target.state == CellState::FLAGGED) return;

    if (target.adjacentNeighbours > 0 &&
        target.state == CellState::SHOWN) {
        std::vector<int> revealableCellsIndexes = GetRevealableCellsIndexes(pressedCellIndex);
        for (auto index : revealableCellsIndexes) RevealCell(index);
    }

    if (target.adjacentNeighbours == 0 && !target.isBomb) {
        RecursiveFill(pressedCellIndex);
        return;
    }

    if (target.Reveal()) revealedCells++;
}

void Board::CountCells()
{
    for (auto&& [index, cell] : enumerate(board))
    {
        if (cell.isBomb) continue;

        SDL_Point position = GetCellPositionByIndex(index);

        int left  =  std::clamp(position.x - 1, 0, boardWidth - 1);
        int top   =  std::clamp(position.y - 1, 0, boardHeight - 1);
        int down  =  std::clamp(position.y + 1, 0, boardHeight - 1);
        int right =  std::clamp(position.x + 1, 0, boardWidth - 1);

        for (int nextCellY = top; nextCellY <= down; nextCellY++) {
            for (int nextCellX = left; nextCellX <= right; nextCellX++) {
                int nextCellIndex = nextCellX + nextCellY * boardWidth;
                if (nextCellX == position.x &&
                    nextCellY == position.y) continue;

                if (GetCell(nextCellIndex).isBomb)
                    cell.adjacentNeighbours++;
            }
        }

    }
}

void Board::RevealBombs()
{
    for (auto& cell : board) {
        if (cell.IsRevealableBomb()) {
            cell.Reveal();
        }
    }
}

void Board::RecursiveFill(int pressedCellIndex)
{
    Cell& target = GetCell(pressedCellIndex);

    if (target.isBomb || target.state == CellState::SHOWN)
        return;

    if (target.Reveal()) revealedCells++;

    if (target.adjacentNeighbours > 0) return;

    SDL_Point position = GetCellPositionByIndex(pressedCellIndex);

    int left  =  std::clamp(position.x - 1, 0, boardWidth - 1);
    int top   =  std::clamp(position.y - 1, 0, boardHeight - 1);
    int down  =  std::clamp(position.y + 1, 0, boardHeight - 1);
    int right =  std::clamp(position.x + 1, 0, boardWidth - 1);

    for (int nextCellY = top; nextCellY <= down; nextCellY++) {
        for (int nextCellX = left; nextCellX <= right; nextCellX++) {
            int nextCellIndex = nextCellX + nextCellY * boardWidth;
            if (nextCellX == position.x &&
                nextCellY == position.y) continue;

            RecursiveFill(nextCellIndex);
        }
    }
}

bool Board::IsCellNeighbours(std::vector<int>& neighbours, int cellIndex)
{
    return std::any_of(neighbours.begin(), neighbours.end(), [&](auto neighbour) {
        return neighbour == cellIndex;
        });
}

std::vector<int> Board::GetNeighboursOfCell(SDL_Point cellPosition)
{
    std::vector<int> neighbours;

    int left  =   std::clamp(cellPosition.x - 1, 0, boardWidth - 1);
    int top   =   std::clamp(cellPosition.y - 1, 0, boardHeight - 1);
    int down  =   std::clamp(cellPosition.y + 1, 0, boardHeight - 1);
    int right =   std::clamp(cellPosition.x + 1, 0, boardWidth - 1);

    for (int nextCellY = top; nextCellY <= down; nextCellY++) {
        for (int nextCellX = left; nextCellX <= right; nextCellX++) {
            if (nextCellX == cellPosition.x &&
                nextCellY == cellPosition.y) continue;

            int nextCellIndex = nextCellX + nextCellY * boardWidth;
            neighbours.emplace_back(nextCellIndex);
        }

    }

    return neighbours;
}

std::vector<int> Board::GetShuffledBoardIndexes(SDL_Point pressedCellPosition)
{
    std::vector<int> shuffledIndexes;
    std::vector<int> neighbours = GetNeighboursOfCell(pressedCellPosition);

    for (int index = 0; index < board.size(); index++) {
        SDL_Point currentCellPosition = GetCellPositionByIndex(index);
        if (!IsCellNeighbours(neighbours, index) &&
            !(currentCellPosition.x == pressedCellPosition.x &&
                currentCellPosition.y == pressedCellPosition.y)) {

            shuffledIndexes.emplace_back(index);
        }
    }

    std::shuffle(shuffledIndexes.begin(), shuffledIndexes.end(), random_mt::mt);
    return shuffledIndexes;
}

int Board::GetCellIndexFromScreenCoordinates(float screenX, float screenY)
{
    int cellX = static_cast<int>(screenX / (constants::CELL_SIZE * constants::SPRITE_SCALE));
    int cellY = static_cast<int>(screenY / (constants::CELL_SIZE * constants::SPRITE_SCALE));
    return cellX + cellY * boardWidth;
}

void Board::PlaceMines(int index)
{

    SDL_Point position = GetCellPositionByIndex(index);
    std::vector<int> shuffledIndexes = GetShuffledBoardIndexes(position);

    for (int index : iota_view(0, bombs)) {
        GetCell(shuffledIndexes[index]).ConvertToMine();
    }
}