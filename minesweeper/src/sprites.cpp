#include "sprites.h"

bool Sprites::Initialize(const std::filesystem::path& path, SDL_Renderer* renderer) {
    //const char* cPath = std::string{ path }.c_str();
    spriteSheet = IMG_LoadTexture(renderer, path.string().c_str());

    if (!spriteSheet) return false;

    for (int sprite = 0; sprite < sprites.size(); sprite++) {
        sprites[sprite] = {
            .x = static_cast<float>(constants::CELL_SIZE * (sprite % constants::SPRITESHEET_COLUMNS)),
            .y = static_cast<float>(constants::CELL_SIZE * static_cast<int>((sprite / constants::SPRITESHEET_COLUMNS))),
            .w = constants::CELL_SIZE,
            .h = constants::CELL_SIZE
        };
    }

    return true;
}

SDL_FRect& Sprites::GetSprite(int index)
{
    return sprites.at(index);
}