#pragma once

#include "texture.h"

struct Button
{
    size_t   x;
    size_t   y;
    Texture* icon;
    void     (*onClick) (void* data);
};

bool createButton  (SDL_Renderer* renderer, Button* button, 
                    size_t x, size_t y, Texture* icon);
void setOnClick    (Button* button, void (*onClick) (void* data));
void renderButton  (SDL_Renderer* renderer, Button* button);
void destroyButton (Button* button);