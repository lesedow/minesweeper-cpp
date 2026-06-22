#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <array>
#include <filesystem>

#include "constants.h"

struct Sprites
{
    SDL_Texture* spriteSheet = nullptr;
    std::array<SDL_FRect, constants::SPRITES_SOURCES_SIZE> sprites{};

    SDL_FRect& GetSprite(int index);
    bool Initialize(const std::filesystem::path& path, SDL_Renderer* renderer);
};