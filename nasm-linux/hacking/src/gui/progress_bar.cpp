#include <assert.h>

#include "progress_bar.h"

void renderProgressBar(SDL_Renderer* renderer, ProgressBar* bar)
{
    assert(renderer);
    assert(bar);

    SDL_Rect rectBorder = {bar->x, 
                           bar->y, 
                           bar->width, 
                           bar->height};

    SDL_SetRenderDrawColor(renderer, 
                           getR(bar->colorBorder), 
                           getG(bar->colorBorder), 
                           getB(bar->colorBorder), 
                           getA(bar->colorBorder));
    SDL_RenderDrawRect(renderer, &rectBorder);

    if (bar->percentage > 0)
    {
        SDL_Rect rectPercentage = {bar->x + bar->margin, 
                                   bar->y + bar->margin, 
                                   (bar->width - 2 * bar->margin) * bar->percentage, 
                                   bar->height - 2 * bar->margin};
       
        SDL_SetRenderDrawColor(renderer, 
                               getR(bar->colorPercentage), 
                               getG(bar->colorPercentage), 
                               getB(bar->colorPercentage), 
                               getA(bar->colorPercentage));  
        SDL_RenderFillRect(renderer, &rectPercentage);

        if (bar->text != nullptr && bar->texture != nullptr)
        {
            textToTexture(bar->texture, bar->text, getSDLColor(bar->colorBorder), bar->font, renderer);
         
            size_t textX = bar->x + (bar->width  / 2) - bar->texture->width  / 2;
            size_t textY = bar->y + (bar->height / 2) - bar->texture->height / 2;
            renderTexture(bar->texture, textX, textY, bar->texture->width, bar->texture->height, renderer);
        }
    }
}