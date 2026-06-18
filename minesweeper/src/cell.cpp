#include "cell.h"

void Cell::Reveal()
{
    state = CellState::SHOWN;
}

bool Cell::IsRevealableBomb()
{
    return (state == CellState::HIDDEN && isBomb);
}

void Cell::ConvertToMine()
{
    isBomb = true;
}

bool Cell::ToggleFlag()
{
    if (state == CellState::HIDDEN) {
        state = CellState::FLAGGED;
        return true;
    }
    else if (state == CellState::FLAGGED) {
        state = CellState::HIDDEN;
        return false;
    }
    return false;
}