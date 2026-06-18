#include "sprites.h"

bool Sprites::Initialize(std::string path, SDL_Renderer* renderer) {
    spriteSheet = IMG_LoadTexture(renderer, path.c_str());

    if (!spriteSheet) return false;

    for (int sprite = 0; sprite < this->sprites.size(); sprite++) {
        sprites[sprite] = {
            .x = static_cast<float>(CELL_SIZE * (sprite % SPRITESHEET_COLUMNS)),
            .y = static_cast<float>(CELL_SIZE * static_cast<int>((sprite / SPRITESHEET_COLUMNS))),
            .w = CELL_SIZE,
            .h = CELL_SIZE
        };
    }

    return true;
}

SDL_FRect& Sprites::GetSprite(int index)
{
    return sprites.at(index);
}