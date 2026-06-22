#include "app_state.h"
#include "constants.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <ranges>
#include <cmath>
#include <iostream>
#include <filesystem>

using namespace std::ranges::views;

AppState::AppState() :menu(&textRenderer){}

SDL_AppResult AppState::Init()
{
    //std::string path = std::format(
    //    "{}assets\\sprites\\minesweeper_sprites.png",
    //    SDL_GetBasePath()
    //);

    //std::string fontPath = std::format(
    //    "{}assets\\fonts\\PixelifySans-Bold.ttf",
    //    SDL_GetBasePath()
    //);

    if (!SDL_CreateWindowAndRenderer(
        "Minesweeper",
        constants::WIDTH,
        constants::HEIGHT,
        SDL_WINDOW_MAXIMIZED,
        &window,
        &renderer
    )) {
        SDL_Log("Couldn't create window and Renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL TTF: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!textRenderer.Initialize(renderer)) return SDL_APP_FAILURE;

    if (!textRenderer.LoadFont(constants::ROBOTO_CONDENSED_PATH)) return SDL_APP_FAILURE;

    if (!gameSprites.Initialize(constants::SPRITE_SHEET_PATH, renderer)) {
        SDL_Log("Couldn't initialize game sprites!: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetTextureScaleMode(gameSprites.spriteSheet, SDL_SCALEMODE_NEAREST);

    return SDL_APP_CONTINUE;
}

SDL_AppResult AppState::Iterate()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (gameState) {
        case State::MENU: menu.Render(renderer, gameSprites); break;
        case State::PLAYING: RenderBoard(); break;
        case State::OVER: break;
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void AppState::HandleGameEvents(SDL_Event* event)
{
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX, mouseY;
        SDL_MouseButtonFlags flag = SDL_GetMouseState(&mouseX, &mouseY);

        int pressedCellIndex = gameBoard.GetCellIndexFromScreenCoordinates(mouseX, mouseY);

        if (flag & SDL_BUTTON_LMASK) { gameBoard.HandleLeftClickedCell(pressedCellIndex); }
        if (flag & SDL_BUTTON_RMASK) { gameBoard.HandleRightClickedCell(pressedCellIndex); }

    }
}

void AppState::HandleMenuEvents(SDL_Event* event)
{
    return;
}

SDL_AppResult AppState::Event(SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    
    if (gameState == State::MENU) HandleMenuEvents(event);
    else if (gameState == State::PLAYING) HandleGameEvents(event);

    return SDL_APP_CONTINUE;
}

void AppState::RenderBoard()
{
    for (const auto& [index, cell] : enumerate(gameBoard.board))
    {
        SDL_Point position = gameBoard.GetCellPositionByIndex(index);

        SDL_FRect destination = {
            .x = static_cast<float>(position.x * constants::CELL_SIZE * constants::SPRITE_SCALE),
            .y = static_cast<float>(position.y * constants::CELL_SIZE * constants::SPRITE_SCALE),
            .w = constants::CELL_SIZE * constants::SPRITE_SCALE,
            .h = constants::CELL_SIZE * constants::SPRITE_SCALE
        };

        SDL_FRect sprite;

        switch (cell.state) {
        case CellState::HIDDEN:
            sprite = gameSprites.GetSprite(CellVisual::HIDDEN_CELL);
            break;
        case CellState::FLAGGED:
            sprite = gameSprites.GetSprite(CellVisual::FLAG);
            break;
        case CellState::SHOWN:
            if (cell.isBomb) {
                sprite = gameSprites.GetSprite(CellVisual::BOMB);
            }
            else if (cell.adjacentNeighbours > 0) {
                int visualIndex = constants::NUMBERS_OFFSET + cell.adjacentNeighbours;
                sprite = gameSprites.GetSprite(visualIndex);
            }
            else {
                sprite = gameSprites.GetSprite(CellVisual::EMPTY);
            }
            break;
        }

        SDL_RenderTexture(
            renderer,
            gameSprites.spriteSheet,
            &sprite,
            &destination
        );
    }
}

void AppState::CleanUp()
{
    SDL_DestroyTexture(gameSprites.spriteSheet);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}