#define SDL_MAIN_USE_CALLBACKS 1

#include "minesweeper.h"

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    AppState* appState = new AppState();
    *appstate = appState;
    return appState->Init();
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    AppState* appState = static_cast<AppState*>(appstate);
    return appState->Event(event);
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* appState = static_cast<AppState*>(appstate);
    return appState->Iterate();
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    AppState* appState = static_cast<AppState*>(appstate);
    appState->CleanUp();
    delete appState;
}
