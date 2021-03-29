#pragma once

#include <stdio.h>

static const size_t DISPLAY_DEFAULT_WIDTH  = 640;
static const size_t DISPLAY_DEFAULT_HEIGHT = 480;
static const char*  DISPLAY_DEFAULT_TITLE  = "Title";
static const size_t DISPLAY_PXL_SIZE       = 1;

struct Display;

Display* newDisplay           (size_t width, size_t height, const char* title);
Display* newDisplay           ();
void     deleteDisplay        (Display* display);
void     updateDisplay        (Display* display, unsigned char* buffer);
void     setDisplayTitle      (Display* display, const char* title);
size_t   getDisplayBufferSize (Display* display);