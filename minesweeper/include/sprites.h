#pragma once

#include <string>

#include <constants.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

struct Sprites
{
    SDL_Texture* spriteSheet = nullptr;
    std::array<SDL_FRect, SPRITES_SOURCES_SIZE> sprites = {};

    SDL_FRect& GetSprite(int index);
    bool Initialize(std::string path, SDL_Renderer* renderer);
};