#pragma once

enum CellState
{
    HIDDEN,
    SHOWN,
    FLAGGED
};

enum CellVisual
{
    HIDDEN_CELL = 0,
    EMPTY = 1,
    FLAG = 2,
    BOMB = 5,
    PRESSED_BOMB = 6
};

struct Cell
{
    CellState state{ CellState::HIDDEN };
    int adjacentNeighbours{};
    bool isBomb{ false };

    bool Reveal();
    bool IsRevealableBomb();
    void ConvertToMine();
    bool ToggleFlag();
};