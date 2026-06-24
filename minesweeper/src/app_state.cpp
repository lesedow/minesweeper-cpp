#include "app_state.h"
#include "constants.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <ranges>
#include <cmath>
#include <iostream>
#include <filesystem>

using namespace std::ranges::views;

SDL_AppResult AppState::Init()
{
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

    if (!textSystem.Initialize(renderer)) return SDL_APP_FAILURE;

    if (!textSystem.LoadFont(constants::ROBOTO_CONDENSED_PATH)) return SDL_APP_FAILURE;

    if (!gameSprites.Initialize(constants::SPRITE_SHEET_PATH, renderer)) {
        SDL_Log("Couldn't initialize game sprites!: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!menu.Initialize(renderer, &textSystem)) return SDL_APP_FAILURE;

    int bW = 16;
    int bH = 16;
    int bC = 40;

    gameBoard.InitializeBoard(bW, bH, bC);

    ResizeWindowAndCenter(bW, bH);

    SDL_SetTextureScaleMode(gameSprites.spriteSheet, SDL_SCALEMODE_NEAREST);

    return SDL_APP_CONTINUE;
}

bool AppState::ResizeWindowAndCenter(int width, int height)
{
    int resizedWidth = width * constants::CELL_SIZE * constants::SPRITE_SCALE;
    int resizedHeight= height * constants::CELL_SIZE * constants::SPRITE_SCALE;

    if (!SDL_SetWindowSize(window, resizedWidth, resizedHeight)) {
        SDL_Log("Failed to resize window: %s", SDL_GetError());
        return false;
    };

    SDL_DisplayID displayId = SDL_GetDisplayForWindow(window);
    if (!displayId) {
        SDL_Log("Failed to get display id: %s", SDL_GetError());
        return false;
    }

    SDL_Rect displayBounds;
    if (!SDL_GetDisplayBounds(displayId, &displayBounds)) {
        SDL_Log("Failed to get display bounds: %s", SDL_GetError());
        return false;
    }

    SDL_SetWindowPosition(
        window,
        displayBounds.w / 2 - resizedWidth / 2,
        displayBounds.h / 2 - resizedHeight / 2
    );
    
    return true;
}

SDL_AppResult AppState::Iterate()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (gameState) {
        case State::MENU: menu.Render(gameSprites); break;
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
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX, mouseY;
        SDL_MouseButtonFlags flag = SDL_GetMouseState(&mouseX, &mouseY);

        int pressedCellIndex = gameBoard.GetCellIndexFromScreenCoordinates(mouseX, mouseY);

        if (flag & SDL_BUTTON_LMASK) { 
            if (menu.state == MenuState::MAIN) {
                SDL_FPoint position = textSystem.GetTransformedText(menu.playButtonText);
                SDL_Point size = textSystem.GetTextSize(menu.playButtonText.text);

                SDL_FRect playButtonBounds{
                    .x = position.x,
                    .y = position.y,
                    .w = position.x + size.x,
                    .h = position.y + size.y,
                };
            }
            
        }

    }
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