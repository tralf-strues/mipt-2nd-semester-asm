#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct Texture
{
    SDL_Texture* sdlTexture;
    size_t       width;
    size_t       height;
};

bool loadTexture    (Texture* texture, const char* filename, SDL_Renderer* renderer);
void destroyTexture (Texture* texture);
bool textToTexture  (Texture* texture, const char* text, SDL_Color color, TTF_Font* font, 
                     SDL_Renderer* renderer);
void renderTexture  (Texture* texture, 
                     size_t x, size_t y, 
                     size_t width, size_t height, 
                     SDL_Renderer* renderer);

uint32_t  getRGBA     (uint32_t r, uint32_t g, uint32_t b, uint32_t a);
SDL_Color getSDLColor (uint32_t color);
char      getR        (uint32_t color);
char      getG        (uint32_t color);
char      getB        (uint32_t color);
char      getA        (uint32_t color);