#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "display.h"

const int      WIDTH                     = 1200;  
const int      HEIGHT                    = 900;
const char*    WINDOW_TITLE              = "Mandelbrot Set";
const size_t   MAX_TITLE_LENGTH          = 64;

const float    HORIZONTAL_MOVE_STEP      = 0.05f; 
const float    VERTICAL_MOVE_STEP        = 0.05f; 
const float    ZOOM_STEP                 = 1.03f;

const float    MANDELBROT_MAX_ITERATIONS = 256;
const float    MANDELBROT_RADIUS_SQUARED = 16; 

const float    MANDELBROT_WIDTH          = 3.5f;
const float    MANDELBROT_HEIGHT         = (float) HEIGHT * (MANDELBROT_WIDTH / WIDTH);

const uint8_t  MANDELBROT_R_START        = 0x07;
const uint8_t  MANDELBROT_G_START        = 0x07;
const uint8_t  MANDELBROT_B_START        = 0;

const float    MANDELBROT_R_POWER        = 1.55f;
const float    MANDELBROT_G_POWER        = 1.54f;
const float    MANDELBROT_B_POWER        = 1.25f;

enum ProgramStatus
{
    FINISHED_NO_ERRORS,
    ERROR,
    RUNNING,
    STOPPED
};

struct Mandelbrot
{
    float centerX;
    float centerY;
    float scale;
};

struct GuiHandler
{
    Display*      display;
    Mandelbrot    mandelbrot;
    uint32_t*     pixelBuffer;
    ProgramStatus status;
};

void     initGuiHandler    (GuiHandler* handler);
void     finish            (GuiHandler* handler);
void     updateWindowTitle (GuiHandler* handler, uint32_t fps);

void     processEvents     (GuiHandler* handler);
void     processKbdEvent   (GuiHandler* handler, const SDL_Event* event);
void     processMouseEvent (GuiHandler* handler, const SDL_Event* event);

void     drawMandelbrotSet (GuiHandler* handler);
uint32_t mapRGBA           (uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

int main()
{
    GuiHandler handler = {};
    initGuiHandler(&handler);

    const uint8_t* keystate = SDL_GetKeyboardState(nullptr);
    assert(keystate);

    while (handler.status == RUNNING)
    {
        uint32_t frameTimeStart = SDL_GetTicks();

        if (keystate[SDL_SCANCODE_UP])    { handler.mandelbrot.centerY += VERTICAL_MOVE_STEP   / (ZOOM_STEP * ZOOM_STEP); }
        if (keystate[SDL_SCANCODE_DOWN])  { handler.mandelbrot.centerY -= VERTICAL_MOVE_STEP   / (ZOOM_STEP * ZOOM_STEP); }
        if (keystate[SDL_SCANCODE_LEFT])  { handler.mandelbrot.centerX -= HORIZONTAL_MOVE_STEP / (ZOOM_STEP * ZOOM_STEP); }
        if (keystate[SDL_SCANCODE_RIGHT]) { handler.mandelbrot.centerX += HORIZONTAL_MOVE_STEP / (ZOOM_STEP * ZOOM_STEP); }

        if (keystate[SDL_SCANCODE_Z])     { handler.mandelbrot.scale   /= ZOOM_STEP; }
        if (keystate[SDL_SCANCODE_X])     { handler.mandelbrot.scale   *= ZOOM_STEP; }

        processEvents(&handler);
        drawMandelbrotSet(&handler);

        updateDisplay(handler.display, (uint8_t*) handler.pixelBuffer);
        updateWindowTitle(&handler, 1000 / (SDL_GetTicks() - frameTimeStart));
    }

    return handler.status;
}

void initGuiHandler(GuiHandler* handler)
{
    assert(handler);

    handler->display = newDisplay(WIDTH, HEIGHT, WINDOW_TITLE); 
    assert(handler->display);

    handler->pixelBuffer = (uint32_t*) calloc(WIDTH * HEIGHT, sizeof(uint32_t));
    assert(handler->pixelBuffer);

    handler->mandelbrot = {0.0f, 0.0f, 1.0f};
    handler->status     = RUNNING;
}

void finish(GuiHandler* handler)
{
    assert(handler);

    free(handler->pixelBuffer);
    deleteDisplay(handler->display);
}

void updateWindowTitle(GuiHandler* handler, uint32_t fps)
{
    assert(handler);

    static char curTitle[MAX_TITLE_LENGTH] = {};    

    snprintf(curTitle, MAX_TITLE_LENGTH, 
                 "%s [FPS: %lu]", WINDOW_TITLE, fps);
    setDisplayTitle(handler->display, curTitle);
}

void processEvents(GuiHandler* handler)
{
    assert(handler);

    SDL_Event event = {};
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            case SDL_QUIT:       { handler->status = STOPPED;           break; }
            case SDL_KEYDOWN:    { processKbdEvent   (handler, &event); break; }
            case SDL_KEYUP:      { processKbdEvent   (handler, &event); break; }
            case SDL_MOUSEWHEEL: { processMouseEvent (handler, &event); break; }
        }
    }
}

void processKbdEvent(GuiHandler* handler, const SDL_Event* event)
{
    assert(handler);
    assert(event);

    switch (event->key.keysym.scancode)
    {
        case SDL_SCANCODE_UP:     { handler->mandelbrot.centerY -= VERTICAL_MOVE_STEP;   break; }
        case SDL_SCANCODE_DOWN:   { handler->mandelbrot.centerY += VERTICAL_MOVE_STEP;   break; }
        case SDL_SCANCODE_LEFT:   { handler->mandelbrot.centerX -= HORIZONTAL_MOVE_STEP; break; }
        case SDL_SCANCODE_RIGHT:  { handler->mandelbrot.centerX += HORIZONTAL_MOVE_STEP; break; }
        case SDL_SCANCODE_ESCAPE: { handler->status              = STOPPED;              break; }
    }
}

void processMouseEvent(GuiHandler* handler, const SDL_Event* event)
{
    assert(handler);
    assert(event);

    handler->mandelbrot.scale += ZOOM_STEP;
}

void drawMandelbrotSet(GuiHandler* handler)
{
    assert(handler);
    assert(handler->pixelBuffer);

    Mandelbrot* mandelbrot = &(handler->mandelbrot);

    float complexStartX = mandelbrot->centerX - mandelbrot->scale * MANDELBROT_WIDTH  / 2;
    float complexStartY = mandelbrot->centerY + mandelbrot->scale * MANDELBROT_HEIGHT / 2;

    for (uint32_t pixelY = 0; pixelY < HEIGHT; pixelY++)
    {
        for (uint32_t pixelX = 0; pixelX < WIDTH; pixelX++)
        {
            float complexX = complexStartX + (mandelbrot->scale * MANDELBROT_WIDTH  / WIDTH)  * pixelX;
            float complexY = complexStartY - (mandelbrot->scale * MANDELBROT_HEIGHT / HEIGHT) * pixelY;

            float curX = complexX;
            float curY = complexY;
            float iterations = 0;
            for (; iterations < MANDELBROT_MAX_ITERATIONS; iterations++)
            {
                float curXSquared = curX * curX;
                float curYSquared = curY * curY;
                float mulCurXCurY = curX * curY;
                
                float curRadiusSquared = curXSquared + curYSquared;
                if (curRadiusSquared >= MANDELBROT_RADIUS_SQUARED) { break; } 

                curX = curXSquared - curYSquared + complexX;
                curY = 2 * mulCurXCurY + complexY;
            }

            float colorIntensity = iterations;

            uint8_t red   = MANDELBROT_R_START + powf(colorIntensity, MANDELBROT_R_POWER);
            uint8_t green = MANDELBROT_G_START + powf(colorIntensity, MANDELBROT_G_POWER);
            uint8_t blue  = MANDELBROT_B_START + powf(colorIntensity, MANDELBROT_B_POWER);
            
            handler->pixelBuffer[pixelY * WIDTH + pixelX] = mapRGBA(red, green, blue, 0xFF);
        }
    }
}

uint32_t mapRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    uint32_t color = red;
    color = (color << 8) + green;
    color = (color << 8) + blue;
    color = (color << 8) + alpha;

    return color;
}
