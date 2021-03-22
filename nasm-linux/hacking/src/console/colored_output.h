#pragma once

#include <stdio.h>

static const size_t MOD_BRIGHT    = 1;
static const size_t MOD_ITALIC    = 3;
static const size_t MOD_UNDERLINE = 4;
static const size_t MOD_FLASHING  = 5;

static const size_t FGR_BLACK     = 30;
static const size_t FGR_RED       = 31;
static const size_t FGR_GREEN     = 32;
static const size_t FGR_YELLOW    = 33;
static const size_t FGR_BLUE      = 34;
static const size_t FGR_PURPLE    = 35;
static const size_t FGR_CYAN      = 36;
static const size_t FGR_WHITE     = 37;

static const size_t BGR_BLACK     = 40;
static const size_t BGR_RED       = 41;
static const size_t BGR_GREEN     = 42;
static const size_t BGR_YELLOW    = 43;
static const size_t BGR_BLUE      = 44;
static const size_t BGR_PURPLE    = 45;
static const size_t BGR_CYAN      = 46;
static const size_t BGR_WHITE     = 47;

void setColor    (size_t modifier, size_t foreground, size_t background);
void setModColor (size_t modifier);
void setFgrColor (size_t foreground);
void setBgrColor (size_t background);
void resetColor  ();