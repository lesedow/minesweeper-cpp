#pragma once

#include <SDL3/SDL.h>

#include <format>

#include "sprites.h"
#include "board.h"
#include "text_system.h"
#include "menu.h"

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

    TextSystem textSystem{};
    State gameState{ State::PLAYING };
    Sprites gameSprites{};
    Board gameBoard{};

    Menu menu{};

    void RenderBoard();

    void CleanUp();
    SDL_AppResult Init();
    SDL_AppResult Iterate();

    bool ResizeWindowAndCenter(int width, int height);

    SDL_AppResult Event(SDL_Event* event);
    void HandleGameEvents(SDL_Event* event);
    void HandleMenuEvents(SDL_Event* event);
};