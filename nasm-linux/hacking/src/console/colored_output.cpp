#include "colored_output.h"

void setColor(size_t modifier, size_t foreground, size_t background)
{
    printf("\e[%lu;%lu;%lum", modifier, foreground, background);
}

void setModColor(size_t modifier)
{
    printf("\e[%lum", modifier);
}

void setFgrColor(size_t foreground)
{
    printf("\e[%lum", foreground);
}

void setBgrColor(size_t background)
{
    printf("\e[%lum", background);
}

void resetColor()
{
    printf("\e[0m\e[0m\e[0m");
}