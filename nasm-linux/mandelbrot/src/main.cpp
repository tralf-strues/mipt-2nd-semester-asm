#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "display.h"

const size_t   PACKAGE_SIZE                = 8;
const __m256   PACKAGE_SIZE_P              = _mm256_set1_ps(PACKAGE_SIZE);

const int      WIDTH                       = 1200;  
const int      HEIGHT                      = 900;
const char*    WINDOW_TITLE                = "Mandelbrot Set";
const size_t   MAX_TITLE_LENGTH            = 64;

const float    HORIZONTAL_MOVE_STEP        = 0.001f; 
const float    VERTICAL_MOVE_STEP          = 0.001f; 
const float    ZOOM_STEP                   = 1.03f;

const size_t   MANDELBROT_MAX_ITERATIONS   = 800;
const __m256   MANDELBROT_MAX_ITERATIONS_P = _mm256_set1_ps(MANDELBROT_MAX_ITERATIONS);
const __m256   MANDELBROT_RADIUS_SQUARED_P = _mm256_set1_ps(4); 

const float    MANDELBROT_WIDTH            = 3.5f;
const float    MANDELBROT_HEIGHT           = (float) HEIGHT * (MANDELBROT_WIDTH / WIDTH);

const __m256   MANDELBROT_MAX_COLOR_P      = _mm256_set1_ps(255.0f);
const __m256   MANDELBROT_R_MULT_P         = _mm256_set1_ps(0.6f);
const __m256   MANDELBROT_G_MULT_P         = _mm256_set1_ps(2.5f);
const __m256   MANDELBROT_B_MULT_P         = _mm256_set1_ps(1.55f);

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
    Display*       display;
    Mandelbrot     mandelbrot;
    uint32_t*      pixelBuffer;
    ProgramStatus  status;

    const uint8_t* keystate;
    int32_t        prevFrameTime;
};

void     initGuiHandler     (GuiHandler* handler);
void     finish             (GuiHandler* handler);
void     updateWindowTitle  (GuiHandler* handler, uint32_t fps);

void     processEvents      (GuiHandler* handler);
void     processKbdEvents   (GuiHandler* handler);

void     drawMandelbrotSet  (GuiHandler* handler);
uint32_t mapRGBA            (uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

int main()
{
    GuiHandler handler = {};
    initGuiHandler(&handler);

    while (handler.status == RUNNING)
    {
        uint32_t frameTimeStart = SDL_GetTicks();

        processEvents(&handler);
        drawMandelbrotSet(&handler);

        updateDisplay(handler.display, (uint8_t*) handler.pixelBuffer);
        
        handler.prevFrameTime = SDL_GetTicks() - frameTimeStart;
        updateWindowTitle(&handler, 1000 / handler.prevFrameTime);
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

    handler->keystate = SDL_GetKeyboardState(nullptr);
    assert(handler->keystate);

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
            case SDL_QUIT: { handler->status = STOPPED; break; }
        }
    }

    processKbdEvents(handler); 
}

void processKbdEvents(GuiHandler* handler)
{
    assert(handler);
    assert(handler->keystate);

    const uint8_t* keystate      = handler->keystate;
    float          prevFrameTime = handler->prevFrameTime;
    Mandelbrot*    mandelbrot    = &(handler->mandelbrot);

    if (keystate[SDL_SCANCODE_UP])     { mandelbrot->centerY += prevFrameTime * VERTICAL_MOVE_STEP   * (mandelbrot->scale); }
    if (keystate[SDL_SCANCODE_DOWN])   { mandelbrot->centerY -= prevFrameTime * VERTICAL_MOVE_STEP   * (mandelbrot->scale); }
    if (keystate[SDL_SCANCODE_LEFT])   { mandelbrot->centerX -= prevFrameTime * HORIZONTAL_MOVE_STEP * (mandelbrot->scale); }
    if (keystate[SDL_SCANCODE_RIGHT])  { mandelbrot->centerX += prevFrameTime * HORIZONTAL_MOVE_STEP * (mandelbrot->scale); }

    if (keystate[SDL_SCANCODE_Z])      { mandelbrot->scale   /= ZOOM_STEP; }
    if (keystate[SDL_SCANCODE_X])      { mandelbrot->scale   *= ZOOM_STEP; }

    if (keystate[SDL_SCANCODE_ESCAPE]) { handler->status      = STOPPED;   }
}

void drawMandelbrotSet(GuiHandler* handler)
{
    assert(handler);
    assert(handler->pixelBuffer);

    Mandelbrot* mandelbrot = &(handler->mandelbrot);

    const __m256 complexStartX     = _mm256_set1_ps(mandelbrot->centerX - mandelbrot->scale * MANDELBROT_WIDTH  / 2);
    const __m256 complexStartY     = _mm256_set1_ps(mandelbrot->centerY + mandelbrot->scale * MANDELBROT_HEIGHT / 2);

    const __m256 scaledWidthRatio  = _mm256_set1_ps(mandelbrot->scale * MANDELBROT_WIDTH  / WIDTH); 
    const __m256 scaledHeightRatio = _mm256_set1_ps(mandelbrot->scale * MANDELBROT_HEIGHT / HEIGHT); 

    for (uint32_t pixelY = 0; pixelY < HEIGHT; pixelY++)
    {
        __m256 pixelY_p = _mm256_set1_ps(pixelY);
        __m256 pixelX_p = _mm256_set_ps(0, 1, 2, 3, 4, 5, 6, 7);

        for (size_t package = 0; package < (WIDTH / PACKAGE_SIZE); package++)
        {
            __m256 complexX = _mm256_add_ps(complexStartX, _mm256_mul_ps(scaledWidthRatio,  pixelX_p));
            __m256 complexY = _mm256_sub_ps(complexStartY, _mm256_mul_ps(scaledHeightRatio, pixelY_p));

            __m256  curX       = complexX;
            __m256  curY       = complexY;
            __m256i iterations = _mm256_setzero_si256();

            for (size_t i = 0; i < MANDELBROT_MAX_ITERATIONS; i++)
            {
                __m256  curXSquared      = _mm256_mul_ps(curX, curX);
                __m256  curYSquared      = _mm256_mul_ps(curY, curY);
                __m256  mulCurXCurY      = _mm256_mul_ps(curX, curY);
                __m256  curRadiusSquared = _mm256_add_ps(curXSquared, curYSquared);
                __m256i areInside        = _mm256_castps_si256(_mm256_cmp_ps(curRadiusSquared, MANDELBROT_RADIUS_SQUARED_P, _CMP_LT_OS));

                if (_mm256_movemask_epi8(areInside) == 0) { break; }  

                curX = _mm256_add_ps(_mm256_sub_ps(curXSquared, curYSquared), complexX);
                curY = _mm256_add_ps(_mm256_add_ps(mulCurXCurY, mulCurXCurY), complexY);

                iterations = _mm256_sub_epi32(iterations, areInside);
            }

            __m256 colorIntensity = _mm256_div_ps(_mm256_cvtepi32_ps(iterations), MANDELBROT_MAX_ITERATIONS_P); 
            colorIntensity = _mm256_mul_ps(_mm256_sqrt_ps(colorIntensity), MANDELBROT_MAX_COLOR_P);

            __m256 sqrtColorIntensity = _mm256_sqrt_ps(colorIntensity);

            __m256 red   = _mm256_mul_ps(_mm256_sub_ps(MANDELBROT_MAX_COLOR_P, sqrtColorIntensity),           MANDELBROT_R_MULT_P);
            __m256 green = _mm256_mul_ps(_mm256_sqrt_ps(_mm256_sqrt_ps(sqrtColorIntensity)) * colorIntensity, MANDELBROT_G_MULT_P);
            __m256 blue  = _mm256_mul_ps(sqrtColorIntensity * colorIntensity,                                 MANDELBROT_B_MULT_P);
            
            float* redColors   = (float*) &red; 
            float* greenColors = (float*) &green; 
            float* blueColors  = (float*) &blue; 

            for (size_t i = 0; i < PACKAGE_SIZE; i++)
            {
                handler->pixelBuffer[pixelY * WIDTH + (package + 1) * PACKAGE_SIZE - i] = mapRGBA(redColors[i], greenColors[i], blueColors[i], 0xFF);
            }

            pixelX_p = _mm256_add_ps(pixelX_p, PACKAGE_SIZE_P);
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
