#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "hash_functions.h"

const uint32_t MURMUR_MUL_COEFFICIENT_1 = 0xCC9E2D51; // c1
const uint32_t MURMUR_MUL_COEFFICIENT_2 = 0x1B873593; // c2
const uint32_t MURMUR_ROT_COEFFICIENT_1 = 15;         // r1
const uint32_t MURMUR_ROT_COEFFICIENT_2 = 13;         // r2

const uint32_t MURMUR_MUL_COEFFICIENT_3 = 5;          // m
const uint32_t MURMUR_ADD_COEFFICIENT   = 0xE6546B64; // n

const uint32_t MURMUR_FINAL_MUL_1       = 0x85EBCA6B; 
const uint32_t MURMUR_FINAL_MUL_2       = 0xC2B2AE35; 
const uint32_t MURMUR_FINAL_SHIFT_RIGHT = 13; 

const uint32_t MURMUR_SEED              = 0x35C2A;    // seed

inline uint32_t leftRotate  (uint32_t value, uint32_t rotation);
inline uint32_t rightRotate (uint32_t value, uint32_t rotation);

uint32_t getConstantHash(const char* string)
{
    assert(string);
    return 0;
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
        hash = rightRotate(hash, 1) ^ tolower(string[i]);
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
        hash = leftRotate(hash, 1) ^ tolower(string[i]);
    }

    return hash;
}

// uint32_t getMurmur3Hash(const char* string)
// {
//     assert(string);
    
//     size_t   length      = strlen(string);
//     uint32_t hash        = MURMUR_SEED;
//     size_t   blocksCount = length / 4;
    
//     const uint32_t* blocks = (const uint32_t*) string;

//     for (uint32_t i = 0; i < blocksCount; i++) {
//         uint32_t block = blocks[i];
//         block *= MURMUR_MUL_COEFFICIENT_1;
//         block = leftRotate(block, MURMUR_ROT_COEFFICIENT_1);
//         block *= MURMUR_MUL_COEFFICIENT_2;

//         hash ^= block;
//         hash = leftRotate(hash, MURMUR_ROT_COEFFICIENT_2) * MURMUR_MUL_COEFFICIENT_3 + MURMUR_ADD_COEFFICIENT;
//     }

//     const uint8_t* tail = (const uint8_t*) (string + blocksCount * 4);
//     uint32_t tailValue = 0;
//     switch (length & 0x3)
//     {
//         case 1: { tailValue ^= tail[0];       break; }
//         case 2: { tailValue ^= tail[1] << 8;  break; }
//         case 3: { tailValue ^= tail[2] << 16; break; }
//     }

//     tailValue *= MURMUR_MUL_COEFFICIENT_1;
//     tailValue = leftRotate(tailValue, MURMUR_ROT_COEFFICIENT_1);
//     tailValue *= MURMUR_MUL_COEFFICIENT_2;
//     hash ^= tailValue;

//     hash ^= length;
//     hash ^= (hash >> 16);
//     hash *= MURMUR_FINAL_MUL_1;
//     hash ^= (hash >> MURMUR_FINAL_SHIFT_RIGHT);
//     hash *= MURMUR_FINAL_MUL_2;
//     hash ^= (hash >> 16);

//     return hash;
// }

inline uint32_t leftRotate(uint32_t value, uint32_t rotation)  
{
    assert(rotation <= 32);

    return (value << rotation) | (value >> (32 - rotation));
}

inline uint32_t rightRotate(uint32_t value, uint32_t rotation)
{
    assert(rotation <= 32);

    return (value >> rotation) | (value << (32 - rotation));
}
