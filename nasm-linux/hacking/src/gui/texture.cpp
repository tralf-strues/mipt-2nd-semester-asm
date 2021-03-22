#include <assert.h>

#include "texture.h"

char getByte(uint32_t number, size_t byteNumber);

bool loadTexture(Texture* texture, const char* filename, SDL_Renderer* renderer)
{
    assert(texture);
    assert(filename);
    assert(renderer);

	texture->sdlTexture = nullptr;
    texture->width      = 0;
    texture->height     = 0;

	SDL_Surface* loadedSurface = IMG_Load(filename);
	if(loadedSurface == nullptr)
	{
		printf("[ERROR] Unable to load image \"%s\"! SDL_image Error: %s\n", 
               filename, 
               IMG_GetError());

        return false;
	}

    texture->sdlTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(texture->sdlTexture == nullptr)
    {
        printf("[ERROR] Unable to create texture from %s! SDL Error: %s\n", 
               filename, 
               SDL_GetError());

        return false;
    }

    texture->width  = loadedSurface->w;
    texture->height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);

	return true;
}

void destroyTexture(Texture* texture)
{
    assert(texture);
    
    texture->width  = 0;
    texture->height = 0;
    if (texture->sdlTexture != nullptr)
    {
        SDL_DestroyTexture(texture->sdlTexture);
    }
}

bool textToTexture(Texture* texture, 
                   const char* text, SDL_Color color, TTF_Font* font, 
                   SDL_Renderer* renderer)
{
    assert(texture);
    assert(text);
    assert(font);
    assert(renderer);

    SDL_Surface* loadedSurface = TTF_RenderText_Blended(font, text, color);
    if (loadedSurface == nullptr)
    {
        printf("[ERROR] Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    texture->sdlTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture->sdlTexture == nullptr)
    {
        printf("[ERROR] Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    texture->width  = loadedSurface->w;
    texture->height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);
    
    return true;
}

void renderTexture(Texture* texture, 
                   size_t x, size_t y, 
                   size_t width, size_t height, 
                   SDL_Renderer* renderer)
{
    assert(texture);   
    assert(renderer);

    SDL_Rect renderQuad = {x, y, width, height};
	SDL_RenderCopy(renderer, texture->sdlTexture, nullptr, &renderQuad);   
}

uint32_t getRGBA(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    return (r << 24) + (g << 16) + (b << 8) + a;
}

SDL_Color getSDLColor(uint32_t color)
{
    return {getR(color), getG(color), getB(color), getA(color)};
}

char getByte(uint32_t number, size_t byteNumber)
{
    return (number & (0xFF << (8 * byteNumber))) >> (8 * byteNumber);
}

char getR(uint32_t color)
{
    return getByte(color, 3);
}

char getG(uint32_t color)
{
    return getByte(color, 2);
}

char getB(uint32_t color)
{
    return getByte(color, 1);
}

char getA(uint32_t color)
{
    return getByte(color, 0);
}
