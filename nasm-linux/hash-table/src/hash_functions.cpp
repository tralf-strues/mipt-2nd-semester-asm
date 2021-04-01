#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "hash_functions.h"

uint32_t getConstantHash(const char* string)
{
    assert(string);
    return 1;
}

uint32_t getStrLenHash(const char* string)
{
    assert(string);
    return strlen(string);
}

uint32_t getFirstCharHash(const char* string)
{
    assert(string);
    return tolower(string[0]);
}

uint32_t getSumCharsHash(const char* string)
{
    assert(string);

    uint32_t hash = 0;

    for (uint32_t i = 0; string[i] != 0; i++)
    {
        hash += tolower(string[i]);
    }

    return hash;
}

uint32_t getXorRightRotateHash(const char* string)
{
    assert(string);

    uint32_t hash = 0;
    
    for (uint32_t i = 0; string[i] != 0; i++)
    {
        // (right circular shift by 1 bit) XOR string[i]
        hash = ((hash >> 1) | (hash << 31)) ^ tolower(string[i]);
    }

    return hash;
}

uint32_t getXorLeftRotateHash(const char* string)
{
    assert(string);

    uint32_t hash = 0;
    
    for (uint32_t i = 0; string[i] != 0; i++)
    {
        // (left circular shift by 1 bit) XOR string[i]
        hash = ((hash << 1) | (hash >> 31)) ^ tolower(string[i]);
    }

    return hash;
}