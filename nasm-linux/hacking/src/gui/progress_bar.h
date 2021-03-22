#pragma once 

#include "texture.h"

struct ProgressBar
{
    float       percentage;
    size_t      x;
    size_t      y;
    size_t      width;
    size_t      height;
    size_t      margin;
    uint32_t    colorBorder;
    uint32_t    colorPercentage;
    const char* text;
    TTF_Font*   font;
    Texture*    texture;
};

void renderProgressBar(SDL_Renderer* renderer, ProgressBar* bar);