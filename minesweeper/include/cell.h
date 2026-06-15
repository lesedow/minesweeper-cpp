#pragma once

enum CellState
{
    HIDDEN,
    SHOWN,
    FLAGGED
};

enum CellVisual
{
    FLAG_CELL,
    HIDDEN_CELL,
    EMPTY_CELL,
    MINE_CELL,
    HIGHLIGHTED_MINE_CELL,
    ONE_CELL,
    TWO_CELL,
    THREE_CELL,
    FOUR_CELL,
    FIVE_CELL,
    SIX_CELL,
    SEVEN_CELL,
    EIGHT_CELL
};

struct Cell
{
    CellState state = CellState::HIDDEN;
    CellVisual visual = CellVisual::EMPTY_CELL;
    int adjacentNeighbours = 0;
    bool isBomb = false;

    void Reveal();
    bool IsRevealableBomb();
    void ConvertToMine();
    bool ToggleFlag();
};