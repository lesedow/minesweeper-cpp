#pragma once

#include <SDL3/SDL.h>

#include <format>

#include "sprites.h"
#include "board.h"
#include "text_renderer.h"

enum State
{
    MENU,
    PLAYING,
    OVER
};

struct AppState
{
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer{ nullptr };

    TextRenderer textRenderer{};
    State gameState{ State::PLAYING };
    Sprites gameSprites{};
    Board gameBoard{};

    void RenderBoard();
    void RenderMenu();

    void CleanUp();
    SDL_AppResult Init();
    SDL_AppResult Iterate();

    SDL_AppResult Event(SDL_Event* event);
    void HandleGameEvents(SDL_Event* event);
    void HandleMenuEvents(SDL_Event* event);
};