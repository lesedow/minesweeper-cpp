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

        if (flag & SDL_BUTTON_LMASK) {
            std::string message{};
            Uint8 messageType{};
            
            BoardState boardState = gameBoard.HandleLeftClickedCell(pressedCellIndex); 
            
            if (boardState == BoardState::WIN) {
                message = "Ai avut noroc!!!";
                messageType = SDL_MESSAGEBOX_INFORMATION;
            }
            else if (boardState == BoardState::LOSE) {
                message = "Esti prost!!!";
                messageType = SDL_MESSAGEBOX_ERROR;
            }

            if (boardState != BoardState::NONE) {
                SDL_ShowSimpleMessageBox(messageType, message.c_str(), message.c_str(), window);
                ResizeWindowAndCenter(constants::EXPERT_W, constants::EXPERT_H);

                gameBoard.Reset();
                gameState = State::MENU;
                menu.state = MenuState::MAIN;
            }

        } else if (flag & SDL_BUTTON_RMASK) 
            gameBoard.HandleRightClickedCell(pressedCellIndex);

    }
}

SDL_AppResult AppState::HandleMenuEvents(SDL_Event* event)
{
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        SDL_FPoint mousePosition{};
        SDL_MouseButtonFlags flag = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        if (flag & SDL_BUTTON_LMASK) { 
            if (menu.state == MenuState::MAIN) {
                SDL_FRect playButtonRect = textSystem.GetTextRect(menu.playButtonText);
                SDL_FRect exitButtonRect = textSystem.GetTextRect(menu.exitButtonText);

                bool isInsidePlayButton = SDL_PointInRectFloat(&mousePosition, &playButtonRect);
                bool isInsideExitButton = SDL_PointInRectFloat(&mousePosition, &exitButtonRect);
                
                if (isInsidePlayButton) {
                    menu.state = MenuState::DIFFICULTY;
                }
                else if (isInsideExitButton) {
                    return SDL_APP_SUCCESS;
                }
            }
            else if (menu.state == MenuState::DIFFICULTY) {
                SDL_FRect easyButtonRect = textSystem.GetTextRect(menu.easyDifficulty);
                SDL_FRect mediumButtonRect = textSystem.GetTextRect(menu.mediumDifficulty);
                SDL_FRect expertButtonRect = textSystem.GetTextRect(menu.expertDifficulty);

                bool isInsideEasyButton = SDL_PointInRectFloat(&mousePosition, &easyButtonRect);
                bool isInsideMediumButton = SDL_PointInRectFloat(&mousePosition, &mediumButtonRect);
                bool isInsideExpertButton = SDL_PointInRectFloat(&mousePosition, &expertButtonRect);
                
                if (isInsideEasyButton ||
                    isInsideMediumButton ||
                    isInsideExpertButton) gameState = State::PLAYING;
                    
                if (isInsideEasyButton) {
                    gameBoard.InitializeBoard(constants::EASY_W, constants::EASY_H, constants::EASY_BOMB_COUNT);
                    ResizeWindowAndCenter(constants::EASY_W, constants::EASY_H); 
                }
                else if (isInsideMediumButton) {
                    gameBoard.InitializeBoard(constants::MEDIUM_W, constants::MEDIUM_H, constants::MEDIUM_BOMB_COUNT);
                    ResizeWindowAndCenter(constants::MEDIUM_W, constants::MEDIUM_H);
                }
                else if (isInsideExpertButton) {
                    gameBoard.InitializeBoard(constants::EXPERT_W, constants::EXPERT_H, constants::EXPERT_BOMB_COUNT);
                    ResizeWindowAndCenter(constants::EXPERT_W, constants::EXPERT_H);
                }

            }
            
        }

    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult AppState::Event(SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    
    if (gameState == State::MENU) return HandleMenuEvents(event);
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
    TTF_DestroyText(menu.menuTitle.text);
    TTF_DestroyText(menu.playButtonText.text);
    TTF_DestroyText(menu.exitButtonText.text);
    TTF_DestroyText(menu.easyDifficulty.text);
    TTF_DestroyText(menu.mediumDifficulty.text);
    TTF_DestroyText(menu.expertDifficulty.text);

    SDL_DestroyTexture(gameSprites.spriteSheet);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}