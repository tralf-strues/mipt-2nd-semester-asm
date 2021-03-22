#include <assert.h>

#include "gui_handler.h"

bool init(GuiHandler* handler, 
          size_t screenWidth, size_t screenHeight, 
          const char* screenTitle, Font font)
{
    assert(handler);
    assert(screenTitle);

    handler->status  = NO_ERROR;
    handler->running = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("[ERROR] SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        
        handler->status = SDL_INIT_ERROR; 
        return false;
    }

    //Initialize PNG loading
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("[ERROR] SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        
        handler->status = SDL_IMG_INIT_ERROR; 
        return false;
    }

    if(TTF_Init() == -1)
    {
        printf("[ERROR] SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    handler->font = TTF_OpenFont(font.filename, font.size);
    if (handler->font == nullptr)
    {
        printf("[ERROR] SDL could not load fonts! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    handler->window = SDL_CreateWindow(screenTitle, 
                                       SDL_WINDOWPOS_UNDEFINED, 
                                       SDL_WINDOWPOS_UNDEFINED, 
                                       screenWidth, 
                                       screenHeight, 
                                       SDL_WINDOW_SHOWN);
    if (handler->window == nullptr)
    {
        printf("[ERROR] Window could not be created! SDL Error: %s\n", SDL_GetError());

        handler->status = WINDOW_CREATE_ERROR; 
        return false;
    }
    
    handler->renderer = SDL_CreateRenderer(handler->window, -1, SDL_RENDERER_SOFTWARE);
    if (handler->renderer == nullptr)
    {
        printf("[ERROR] Renderer could not be created! SDL Error: %s\n", SDL_GetError());

        handler->status = RENDERER_CREATE_ERROR;
        return false;
    }
    
	return true;
}

void run(GuiHandler* handler,
         void* data,
         void (*processEvent)(GuiHandler* handler, const SDL_Event* event, void* data),
         void (*update)      (GuiHandler* handler, void* data))
{
    assert(handler);
    assert(processEvent);
    assert(update);

    // TODO: add fps counter here

    handler->running = true;

    SDL_Event event = {};
    while (handler->running)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            processEvent(handler, &event, data);
        }

        update(handler, data);
    }
}

void close(GuiHandler* handler)
{
    assert(handler);

	SDL_DestroyRenderer(handler->renderer);
	SDL_DestroyWindow(handler->window);

    handler->status       = NO_ERROR;
    handler->running      = false;
	handler->window       = nullptr;
	handler->renderer     = nullptr;
	handler->font         = nullptr;
	handler->screenWidth  = 0;
	handler->screenHeight = 0;
	handler->screenTitle  = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

