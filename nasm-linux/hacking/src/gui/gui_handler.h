#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

enum GuiError
{
    NO_ERROR,
    SDL_INIT_ERROR,
    SDL_IMG_INIT_ERROR,
    WINDOW_CREATE_ERROR,
    RENDERER_CREATE_ERROR,
    ERRORS_TOTAL
};

struct GuiHandler
{
    GuiError      status;
    bool          running;

    SDL_Window*   window;
    SDL_Renderer* renderer;
    TTF_Font*     font;

    size_t        screenWidth;
    size_t        screenHeight;
    const char*   screenTitle;
};

struct Font
{
    const char* filename;
    size_t      size;
};

bool init  (GuiHandler* handler, 
            size_t screenWidth, size_t screenHeight, 
            const char* screenTitle, Font font);

void close (GuiHandler* handler);

void run   (GuiHandler* handler,
            void* data,
            void (*processEvent)(GuiHandler* handler, const SDL_Event* event, void* data),
            void (*update)      (GuiHandler* handler, void* data));