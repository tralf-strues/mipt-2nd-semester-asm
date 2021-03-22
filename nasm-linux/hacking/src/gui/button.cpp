#include <assert.h>

#include "button.h"

bool createButton(SDL_Renderer* renderer, Button* button, 
                  size_t x, size_t y, Texture* icon)
{
    assert(renderer);
    assert(button);
    assert(icon);

    button->x    = x;
    button->y    = y;
    button->icon = icon;

    return true;
}

void setOnClick(Button* button, void (*onClick) (void* data))
{
    assert(button);
    button->onClick = onClick;
}

void renderButton(SDL_Renderer* renderer, Button* button)
{
    assert(renderer);
    assert(button);
    assert(button->icon);

    renderTexture(button->icon, button->x, button->y, 
                  button->icon->width, button->icon->height,
                  renderer);
}

void destroyButton(Button* button)
{
    assert(button);

    button->x       = 0;
    button->y       = 0;
    button->onClick = nullptr;

    if (button->icon != nullptr)
    {
        destroyTexture(button->icon);
        button->icon = nullptr;
    }
}