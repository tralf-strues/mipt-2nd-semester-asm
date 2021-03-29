#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "display.h"

struct Display
{
    size_t        width    = 0;
    size_t        height   = 0;
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  texture  = nullptr;
};

Display* newDisplay(size_t width, size_t height, const char* title)
{
    assert(width  > 0);
    assert(height > 0);
    assert(title);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return nullptr;
    }
    
    Display* display = (Display*) calloc(1, sizeof(Display));
    assert(display);

    display->width  = width;
    display->height = height;

    display->window = SDL_CreateWindow(title,
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       DISPLAY_PXL_SIZE * width, DISPLAY_PXL_SIZE * height,
                                       0);
    assert(display->window);

    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
    assert(display->renderer);

    display->texture = SDL_CreateTexture(display->renderer,
                                         SDL_PIXELFORMAT_RGBA8888, 
                                         SDL_TEXTUREACCESS_STREAMING,
                                         width, height);
    assert(display->texture);

    return display;
}

Display* newDisplay()
{
    return newDisplay(DISPLAY_DEFAULT_WIDTH, DISPLAY_DEFAULT_HEIGHT, DISPLAY_DEFAULT_TITLE);
}

void deleteDisplay(Display* display)
{
    if (display == nullptr) { return; }

    display->width    = 0;
    display->height   = 0;
    display->window   = nullptr;
    display->renderer = 0;
    display->texture  = 0;

    SDL_DestroyTexture  (display->texture);
    SDL_DestroyRenderer (display->renderer);
    SDL_DestroyWindow   (display->window);

    free(display);
}

void updateDisplay(Display* display, unsigned char* buffer)
{
    assert(display);
    assert(display->width  > 0);
    assert(display->height > 0);
    assert(display->window);
    assert(display->renderer);
    assert(display->texture);
    assert(buffer);

    SDL_Event event = {};
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) 
        {
            exit(0);
        }
    }

    SDL_RenderClear(display->renderer);
    SDL_UpdateTexture(display->texture, 
                      nullptr, 
                      (void*) buffer, 
                      display->width * 4); // 4 for RGBA

    SDL_RenderCopy(display->renderer, display->texture, nullptr, nullptr);
    SDL_RenderPresent(display->renderer);
}

void setDisplayTitle(Display* display, const char* title)
{
    assert(display);
    assert(title);

    SDL_SetWindowTitle(display->window, title);
}

size_t getDisplayBufferSize(Display* display)
{
    assert(display != nullptr);

    return display->width * display->height * 4;
}

